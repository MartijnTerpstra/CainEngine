#include "Precomp.h"

#include "WaylandWindow.h"

#include "EnumConverter.h"

#include <cstring>
#include <poll.h>
#include <sys/mman.h>
#include <unistd.h>

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

namespace
{

struct BoundGlobals
{
	wl_compositor* compositor = nullptr;
	xdg_wm_base* wmBase = nullptr;
	wl_seat* seat = nullptr;
};

void handleRegistryGlobal(
	void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t /*version*/)
{
	auto& globals = *static_cast<BoundGlobals*>(data);

	// Bind everything at version 1: it's the only version guaranteed to be supported, and the
	// requests/events this backend uses (create_surface, get_xdg_surface/get_toplevel,
	// ping/pong, set_title, configure/close, capabilities, the keyboard events) are all
	// baseline v1 functionality.
	if(std::strcmp(interface, wl_compositor_interface.name) == 0)
	{
		globals.compositor = static_cast<wl_compositor*>(
			wl_registry_bind(registry, name, &wl_compositor_interface, 1));
	}
	else if(std::strcmp(interface, xdg_wm_base_interface.name) == 0)
	{
		globals.wmBase =
			static_cast<xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
	}
	else if(std::strcmp(interface, wl_seat_interface.name) == 0)
	{
		globals.seat = static_cast<wl_seat*>(wl_registry_bind(registry, name, &wl_seat_interface, 1));
	}
}

void handleRegistryGlobalRemove(void*, wl_registry*, uint32_t)
{ }

const wl_registry_listener g_registryListener = {
	.global = &handleRegistryGlobal,
	.global_remove = &handleRegistryGlobalRemove,
};

const xdg_wm_base_listener g_wmBaseListener = {
	.ping = &WaylandWindow::handleXdgWmBasePing,
};

const xdg_surface_listener g_xdgSurfaceListener = {
	.configure = &WaylandWindow::handleXdgSurfaceConfigure,
};

const xdg_toplevel_listener g_xdgToplevelListener = {
	.configure = &WaylandWindow::handleXdgToplevelConfigure,
	.close = &WaylandWindow::handleXdgToplevelClose,
};

const wl_callback_listener g_frameListener = {
	.done = &WaylandWindow::handleFrameDone,
};

// wl_seat_listener::name is version-2+; the seat is bound at version 1, so the compositor never
// sends it and leaving that callback unset (null) here is safe.
const wl_seat_listener g_seatListener = {
	.capabilities = &WaylandWindow::handleSeatCapabilities,
};

const wl_keyboard_listener g_keyboardListener = {
	.keymap = &WaylandWindow::handleKeyboardKeymap,
	.enter = &WaylandWindow::handleKeyboardEnter,
	.leave = &WaylandWindow::handleKeyboardLeave,
	.key = &WaylandWindow::handleKeyboardKey,
	.modifiers = &WaylandWindow::handleKeyboardModifiers,
};

} // namespace

WaylandWindow::WaylandWindow(wl_display* display, wl_surface* surface, xdg_wm_base* wmBase,
	xdg_surface* xdgSurface, xdg_toplevel* xdgToplevel, wl_seat* seat, const uint2& size,
	std::string name, const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
	ClientInterfaces::IWindowEventListener* listenerPointer)
	: m_display(display)
	, m_surface(surface)
	, m_wmBase(wmBase)
	, m_xdgSurface(xdgSurface)
	, m_xdgToplevel(xdgToplevel)
	, m_seat(seat)
	, m_size(size)
	, m_xkbContext(xkb_context_new(XKB_CONTEXT_NO_FLAGS))
	, m_name(std::move(name))
	, m_listener(listener)
	, m_listenerPointer(listenerPointer)
{ }

WaylandWindow::~WaylandWindow()
{
	detachKeyboard();

	if(m_xkbContext != nullptr)
		xkb_context_unref(m_xkbContext);

	if(m_seat != nullptr)
		wl_seat_destroy(m_seat);

	xdg_toplevel_destroy(m_xdgToplevel);
	xdg_surface_destroy(m_xdgSurface);
	xdg_wm_base_destroy(m_wmBase);
	wl_surface_destroy(m_surface);

	wl_display_disconnect(m_display);
}

RefPtr<IWindow> WaylandWindow::createNewWindow(const std::string& name, const uint2& size,
	WindowType /*type*/, flag<WindowFlags> /*flags*/,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
	ClientInterfaces::IWindowEventListener* listenerPointer)
{
	wl_display* display = wl_display_connect(nullptr);
	if(display == nullptr)
	{
		Common::fatalError("WaylandWindow::createNewWindow(): wl_display_connect failed");
	}

	wl_registry* registry = wl_display_get_registry(display);

	BoundGlobals globals;
	wl_registry_add_listener(registry, &g_registryListener, &globals);

	wl_display_roundtrip(display);

	wl_registry_destroy(registry);

	if(globals.compositor == nullptr || globals.wmBase == nullptr)
	{
		Common::fatalError(
			"WaylandWindow::createNewWindow(): compositor is missing wl_compositor/xdg_wm_base");
	}

	wl_surface* surface = wl_compositor_create_surface(globals.compositor);

	// Not needed past surface creation - see the corresponding note about m_wmBase/m_seat below
	// on why those two globals *are* kept around for the window's lifetime.
	wl_compositor_destroy(globals.compositor);

	xdg_surface* xdgSurface = xdg_wm_base_get_xdg_surface(globals.wmBase, surface);
	xdg_toplevel* xdgToplevel = xdg_surface_get_toplevel(xdgSurface);

	xdg_toplevel_set_title(xdgToplevel, name.c_str());

	auto window = RefPtr<WaylandWindow>::create(display, surface, globals.wmBase, xdgSurface,
		xdgToplevel, globals.seat, size, name, listener, listenerPointer);

	// xdg_wm_base pings for as long as the connection is open and must always be pong'd back,
	// and wl_seat's capabilities can change at any time (e.g. a keyboard being (un)plugged) -
	// both listeners, and the objects they're on, need to stay alive for the window's lifetime,
	// unlike the wl_compositor above which is only needed once, to create the surface.
	xdg_wm_base_add_listener(globals.wmBase, &g_wmBaseListener, window.get());
	xdg_surface_add_listener(xdgSurface, &g_xdgSurfaceListener, window.get());
	xdg_toplevel_add_listener(xdgToplevel, &g_xdgToplevelListener, window.get());

	if(globals.seat != nullptr)
	{
		wl_seat_add_listener(globals.seat, &g_seatListener, window.get());
	}

	// Commit once with no buffer attached: this is what asks the compositor to assign the
	// xdg_toplevel role and start the configure handshake xdg-shell requires (see
	// handleXdgSurfaceConfigure()). No buffer ever gets attached here, on purpose - unlike an
	// X11 window (which the server paints its background_pixel into as soon as it's mapped, see
	// XorgWindow::createNewWindow()), a wl_surface only becomes visible once *something* attaches
	// a real buffer to it, and that's meant to be a Vulkan swap chain's presentation engine
	// (vkCreateWaylandSurfaceKHR takes exactly the wl_display/wl_surface pair getDisplay()/
	// getSurface() expose) - not this backend reaching in and attaching one of its own.
	wl_surface_commit(surface);
	wl_display_roundtrip(display);

	return window;
}

void WaylandWindow::show()
{
	m_shown = true;

	// Nothing will actually be visible until a swap chain attaches and presents a real buffer
	// (see the note in createNewWindow()); this commit only matters for any other surface state
	// a listener may have changed since.
	wl_surface_commit(m_surface);
	wl_display_flush(m_display);
}

void WaylandWindow::redraw()
{
	// Mirrors XorgWindow::redraw(): schedule the notification to the listener rather than
	// calling it inline, so it's delivered from handleEvents() like every other event.
	wl_callback* callback = wl_surface_frame(m_surface);
	wl_callback_add_listener(callback, &g_frameListener, this);

	wl_surface_commit(m_surface);
	wl_display_flush(m_display);
}

void WaylandWindow::maximize()
{
	Common::fatalError("Not implemented");
}

void WaylandWindow::minimize()
{
	Common::fatalError("Not implemented");
}

void WaylandWindow::close()
{
	// Unlike XorgWindow::close(), this doesn't destroy the Wayland objects here: destroying a
	// wl_proxy-backed object (xdg_toplevel_destroy() and friends) frees it immediately client
	// side, so doing that again in the destructor would be a real double-free rather than X11's
	// tolerated no-op-on-a-stale-resource-ID. All of that teardown happens exactly once, below.
	m_shown = false;
}

void WaylandWindow::handleEvents()
{
	// Non-blocking pump of whatever the compositor already has queued, mirroring
	// XorgWindow::handleEvents()'s XPending()-gated loop: only read new data off the display's
	// socket if there is some ready (poll with a zero timeout), then dispatch it.
	while(wl_display_prepare_read(m_display) != 0)
	{
		wl_display_dispatch_pending(m_display);
	}

	wl_display_flush(m_display);

	pollfd pfd = {};
	pfd.fd = wl_display_get_fd(m_display);
	pfd.events = POLLIN;

	if(poll(&pfd, 1, 0) > 0 && (pfd.revents & POLLIN) != 0)
	{
		wl_display_read_events(m_display);
	}
	else
	{
		wl_display_cancel_read(m_display);
	}

	wl_display_dispatch_pending(m_display);
}

bool WaylandWindow::isShown() const
{
	return m_shown;
}

std::string WaylandWindow::getName() const
{
	return m_name;
}

int WaylandWindow::getWidth() const
{
	return int(m_size.x);
}

int WaylandWindow::getHeight() const
{
	return int(m_size.y);
}

Rect WaylandWindow::getRect() const
{
	// Unlike XorgWindow (XGetWindowAttributes), Wayland gives clients no on-screen position for
	// their own surface at all - only a size (see handleXdgToplevelConfigure()) - so this is
	// surface-local, origin always at (0, 0).
	return Rect(0, 0, int(m_size.x), int(m_size.y));
}

Rect WaylandWindow::getClientRect() const
{
	// No client-side decorations are drawn by this window (xdg-shell without them just leaves
	// decoration to the compositor, or draws none), so the client area is the whole surface.
	return getRect();
}

void WaylandWindow::toForeground()
{
	Common::fatalError("Not implemented");
}

::wl_display* WaylandWindow::getDisplay() const
{
	return m_display;
}

::wl_surface* WaylandWindow::getSurface() const
{
	return m_surface;
}

void WaylandWindow::handleXdgWmBasePing(void*, xdg_wm_base* wmBase, uint32_t serial)
{
	xdg_wm_base_pong(wmBase, serial);
}

void WaylandWindow::handleXdgSurfaceConfigure(void* data, xdg_surface* xdgSurface, uint32_t serial)
{
	xdg_surface_ack_configure(xdgSurface, serial);

	// Nothing of ours to (re)commit in response - see the note in createNewWindow() about why no
	// buffer is ever attached by this class - so there's no equivalent of the old
	// attach/damage/commit sequence here; a swap chain's own presentation is what commits from
	// here on.
}

void WaylandWindow::handleXdgToplevelConfigure(
	void* data, xdg_toplevel*, int32_t width, int32_t height, wl_array* /*states*/)
{
	// A width/height of zero means "you decide" (e.g. the very first configure, before the
	// compositor has an opinion) - keep whatever size is already set (the size requested at
	// creation, or the last one a real configure assigned) rather than treating it as 0x0.
	if(width <= 0 || height <= 0)
		return;

	auto& window = *static_cast<WaylandWindow*>(data);

	if(window.m_size.x == uint32_t(width) && window.m_size.y == uint32_t(height))
		return;

	window.m_size = uint2(uint32_t(width), uint32_t(height));

	// This is the only way this backend learns of a size change - Wayland has no equivalent of
	// polling a window's current size (see getRect()) - so a swap chain has to listen for this
	// to know when to recreate itself with a new extent.
	auto listener = window.m_listener.lock();
	auto* rawListener = listener != nullptr ? listener.get() : window.m_listenerPointer;

	if(rawListener != nullptr)
	{
		rawListener->onResize(&window, window.m_size);
	}
}

void WaylandWindow::handleXdgToplevelClose(void* data, xdg_toplevel*)
{
	static_cast<WaylandWindow*>(data)->m_shown = false;
}

void WaylandWindow::handleFrameDone(void* data, wl_callback* callback, uint32_t /*time*/)
{
	wl_callback_destroy(callback);

	auto& window = *static_cast<WaylandWindow*>(data);

	auto listener = window.m_listener.lock();
	auto* rawListener = listener != nullptr ? listener.get() : window.m_listenerPointer;

	if(rawListener != nullptr)
	{
		rawListener->onRedraw(&window);
	}
}

void WaylandWindow::handleSeatCapabilities(void* data, wl_seat* seat, uint32_t capabilities)
{
	auto& window = *static_cast<WaylandWindow*>(data);

	const bool hasKeyboard = (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) != 0;

	if(hasKeyboard && window.m_keyboard == nullptr)
	{
		window.attachKeyboard(wl_seat_get_keyboard(seat));
	}
	else if(!hasKeyboard && window.m_keyboard != nullptr)
	{
		window.detachKeyboard();
	}
}

void WaylandWindow::handleKeyboardKeymap(
	void* data, wl_keyboard*, uint32_t format, int32_t fd, uint32_t size)
{
	auto& window = *static_cast<WaylandWindow*>(data);

	if(format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1 || window.m_xkbContext == nullptr)
	{
		::close(fd);
		return;
	}

	void* mapping = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(mapping == MAP_FAILED)
	{
		::close(fd);
		Common::fatalError("WaylandWindow: mmap of the keyboard keymap failed");
	}

	if(window.m_xkbState != nullptr)
	{
		xkb_state_unref(window.m_xkbState);
		window.m_xkbState = nullptr;
	}
	if(window.m_xkbKeymap != nullptr)
	{
		xkb_keymap_unref(window.m_xkbKeymap);
	}

	window.m_xkbKeymap = xkb_keymap_new_from_string(window.m_xkbContext,
		static_cast<const char*>(mapping), XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);

	munmap(mapping, size);
	::close(fd);

	if(window.m_xkbKeymap == nullptr)
	{
		Common::fatalError("WaylandWindow: failed to compile the keyboard keymap");
	}

	window.m_xkbState = xkb_state_new(window.m_xkbKeymap);
}

void WaylandWindow::handleKeyboardEnter(void*, wl_keyboard*, uint32_t, wl_surface*, wl_array*)
{ }

void WaylandWindow::handleKeyboardLeave(void*, wl_keyboard*, uint32_t, wl_surface*)
{ }

void WaylandWindow::handleKeyboardKey(
	void* data, wl_keyboard*, uint32_t /*serial*/, uint32_t /*time*/, uint32_t key, uint32_t state)
{
	static_cast<WaylandWindow*>(data)->handleKey(key, state);
}

void WaylandWindow::handleKeyboardModifiers(void* data, wl_keyboard*, uint32_t /*serial*/,
	uint32_t modsDepressed, uint32_t modsLatched, uint32_t modsLocked, uint32_t group)
{
	auto& window = *static_cast<WaylandWindow*>(data);

	if(window.m_xkbState == nullptr)
		return;

	xkb_state_update_mask(window.m_xkbState, modsDepressed, modsLatched, modsLocked, 0, 0, group);
}

void* WaylandWindow::asImpl(uint64_t typeHash) const
{
	switch(typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(IWindow);
		CHECK_TYPE_AND_RETURN(Linux::IWaylandWindow);
		CHECK_TYPE_AND_RETURN(WaylandWindow);
	default:
		return nullptr;
	}
}

void WaylandWindow::attachKeyboard(wl_keyboard* keyboard)
{
	m_keyboard = keyboard;
	wl_keyboard_add_listener(m_keyboard, &g_keyboardListener, this);
}

void WaylandWindow::detachKeyboard()
{
	if(m_keyboard == nullptr)
		return;

	if(m_xkbState != nullptr)
	{
		xkb_state_unref(m_xkbState);
		m_xkbState = nullptr;
	}
	if(m_xkbKeymap != nullptr)
	{
		xkb_keymap_unref(m_xkbKeymap);
		m_xkbKeymap = nullptr;
	}

	wl_keyboard_release(m_keyboard);
	m_keyboard = nullptr;
}

void WaylandWindow::handleKey(uint32_t key, uint32_t state)
{
	if(state != WL_KEYBOARD_KEY_STATE_PRESSED)
	{
		// On Keyup
		return;
	}

	if(m_xkbState == nullptr)
		return;

	// The wire protocol sends evdev keycodes; XKB (and X11) keycodes are offset by 8.
	auto keysym = xkb_state_key_get_one_sym(m_xkbState, key + 8);

	auto keyCode = EnumConverter::toKeyCodes(keysym);

	mst::flag<KeyModifiers> modifiers;

	if(xkb_state_mod_name_is_active(m_xkbState, XKB_MOD_NAME_CTRL, XKB_STATE_MODS_EFFECTIVE) > 0)
		modifiers.enable(KeyModifiers::Ctrl);

	if(xkb_state_mod_name_is_active(m_xkbState, XKB_MOD_NAME_SHIFT, XKB_STATE_MODS_EFFECTIVE) > 0)
		modifiers.enable(KeyModifiers::Shift);

	if(xkb_state_mod_name_is_active(m_xkbState, XKB_MOD_NAME_CAPS, XKB_STATE_MODS_EFFECTIVE) > 0)
		modifiers.enable(KeyModifiers::CapsLock);

	auto listener = m_listener.lock();
	auto* rawListener = listener != nullptr ? listener.get() : m_listenerPointer;

	if(rawListener != nullptr)
	{
		rawListener->onKeyDown(this, keyCode, modifiers);
	}
}
