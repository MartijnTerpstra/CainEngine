#pragma once

// Not part of the public External/Platform.Linux.h umbrella (unlike <wayland-client.h>, which
// IWaylandWindow's interface needs) - xdg-shell and XKB are implementation details of this one
// backend, generated/found via Platform.Linux/CMakeLists.txt.
#include "xdg-shell-client-protocol.h"

#include <xkbcommon/xkbcommon.h>

namespace CainEngine {
namespace Platform {
namespace Internal {

class WaylandWindow final : public Linux::IWaylandWindow
{
	friend class Common::RefPtr<WaylandWindow>;

private:
	// ctor & dtor

	WaylandWindow(wl_display* display, wl_surface* surface, xdg_wm_base* wmBase,
		xdg_surface* xdgSurface, xdg_toplevel* xdgToplevel, wl_seat* seat, const uint2& size,
		std::string name, const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
		ClientInterfaces::IWindowEventListener* listenerPointer);
	~WaylandWindow();

	COMMON_DECLARE_NON_COPY(WaylandWindow);

public:
	// Creation

	static RefPtr<IWindow> createNewWindow(const std::string& name, const uint2& size,
		WindowType type, flag<WindowFlags> flags,
		const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
		ClientInterfaces::IWindowEventListener* listenerPointer);

public:
	// IWindow overrides

	void show() override;

	void redraw() override;

	void maximize() override;

	void minimize() override;

	void close() override;

	void handleEvents() override;

	bool isShown() const override;

	std::string getName() const override;

	int getWidth() const override;

	int getHeight() const override;

	Rect getRect() const override;

	Rect getClientRect() const override;

	void toForeground() override;

public:
	// IWaylandWindow overrides

	::wl_display* getDisplay() const override;

	::wl_surface* getSurface() const override;

public:
	// Callback trampolines for the wl_registry/xdg-shell/wl_seat/wl_keyboard C APIs; these are
	// registered as *_listener function pointers, so they have to be reachable from outside the
	// class (the actual per-instance handling they do is delegated to private members below).

	static void handleXdgWmBasePing(void* data, xdg_wm_base* wmBase, uint32_t serial);
	static void handleXdgSurfaceConfigure(void* data, xdg_surface* xdgSurface, uint32_t serial);
	static void handleXdgToplevelConfigure(
		void* data, xdg_toplevel* toplevel, int32_t width, int32_t height, wl_array* states);
	static void handleXdgToplevelClose(void* data, xdg_toplevel* toplevel);
	static void handleFrameDone(void* data, wl_callback* callback, uint32_t time);
	static void handleSeatCapabilities(void* data, wl_seat* seat, uint32_t capabilities);
	static void handleKeyboardKeymap(
		void* data, wl_keyboard* keyboard, uint32_t format, int32_t fd, uint32_t size);
	static void handleKeyboardEnter(
		void* data, wl_keyboard* keyboard, uint32_t serial, wl_surface* surface, wl_array* keys);
	static void handleKeyboardLeave(
		void* data, wl_keyboard* keyboard, uint32_t serial, wl_surface* surface);
	static void handleKeyboardKey(void* data, wl_keyboard* keyboard, uint32_t serial,
		uint32_t time, uint32_t key, uint32_t state);
	static void handleKeyboardModifiers(void* data, wl_keyboard* keyboard, uint32_t serial,
		uint32_t modsDepressed, uint32_t modsLatched, uint32_t modsLocked, uint32_t group);

private:
	// BaseObject overrides

	void* asImpl(uint64_t) const override;

private:
	// Internal functionality

	void attachKeyboard(wl_keyboard* keyboard);
	void detachKeyboard();

	void handleKey(uint32_t key, uint32_t state);

private:
	// Member variables

	wl_display* const m_display;
	wl_surface* const m_surface;
	xdg_wm_base* const m_wmBase;
	xdg_surface* const m_xdgSurface;
	xdg_toplevel* const m_xdgToplevel;
	wl_seat* const m_seat;
	uint2 m_size;
	wl_keyboard* m_keyboard = nullptr;
	xkb_context* m_xkbContext = nullptr;
	xkb_keymap* m_xkbKeymap = nullptr;
	xkb_state* m_xkbState = nullptr;
	const std::string m_name;
	const std::weak_ptr<ClientInterfaces::IWindowEventListener> m_listener;
	ClientInterfaces::IWindowEventListener* const m_listenerPointer;
	bool m_shown = false;

}; // class WaylandWindow

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine
