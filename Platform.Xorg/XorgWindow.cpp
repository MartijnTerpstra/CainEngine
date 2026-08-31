#include "Precomp.h"

#include "XorgWindow.h"

#include "EnumConverter.h"

#include <X11/XKBlib.h>
#include <X11/Xutil.h>

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

XorgWindow::XorgWindow(Display* display, ::Window window, string name,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
	ClientInterfaces::IWindowEventListener* listenerPointer)
	: m_display(display),
	m_window(window),
	m_name(move(name)),
	m_listener(listener),
	m_listenerPointer(listenerPointer)
{
	COMMON_CALLSTACK_CALL;
}

XorgWindow::~XorgWindow()
{
	COMMON_CALLSTACK_CALL;

	XDestroyWindow(m_display, m_window);

	XCloseDisplay(m_display);
}

RefPtr<IWindow> XorgWindow::createNewWindow(const string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
	ClientInterfaces::IWindowEventListener* listenerPointer)
{
	COMMON_CALLSTACK_CALL;

	Display* display = XOpenDisplay(":0.0");
	if (display == null)
	{
		Common::fatalError("XorgWindow::createNewWindow(): XOpenDisplay failed");
	}

	long visualMask = VisualScreenMask;
	int numberOfVisuals;
	XVisualInfo vInfoTemplate = {};
	vInfoTemplate.screen = XDefaultScreen(display);
	XVisualInfo *visualInfo = XGetVisualInfo(display, visualMask,
		&vInfoTemplate, &numberOfVisuals);

	Colormap colormap = XCreateColormap(
		display, XRootWindow(display, vInfoTemplate.screen),
		visualInfo->visual, AllocNone);

	XSetWindowAttributes windowAttributes = {};
	windowAttributes.colormap = colormap;
	windowAttributes.background_pixel = 0xFFFFFFFF;
	windowAttributes.border_pixel = 0;
	windowAttributes.event_mask =
		/*KeyPressMask | KeyReleaseMask | StructureNotifyMask | ExposureMask | VisibilityNotify*/ 0xFFFFFF;

	Window window = XCreateWindow(
		display, XRootWindow(display, vInfoTemplate.screen), 0, 0,
		size.x, size.y, 0, visualInfo->depth, InputOutput,
		visualInfo->visual,
		CWBackPixel | CWBorderPixel | CWEventMask | CWColormap, &windowAttributes);

	XStoreName(display, window, name.c_str());

	//XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask | VisibilityNotify);

	return Common::RefPtr<XorgWindow>::Create(display, window, name, listener, listenerPointer);
}

void XorgWindow::show()
{
	COMMON_CALLSTACK_CALL;

	m_shown = true;

	XMapWindow(m_display, m_window);
	XFlush(m_display);

	m_deleteWindowAtom =
		XInternAtom(m_display, "WM_DELETE_WINDOW", False);
}

void XorgWindow::redraw()
{
	COMMON_CALLSTACK_CALL;

	XEvent evt = {};

	evt.type = Expose;

	XSendEvent(m_display, m_window, True, ExposureMask, &evt);
}

void XorgWindow::maximize()
{
	COMMON_CALLSTACK_CALL;

	Common::fatalError("Not implemented");
}

void XorgWindow::minimize()
{
	COMMON_CALLSTACK_CALL;

	Common::fatalError("Not implemented");
}

void XorgWindow::close()
{
	COMMON_CALLSTACK_CALL;

	m_shown = false;
	XDestroyWindow(m_display, m_window);
}

void XorgWindow::handleEvents()
{
	COMMON_CALLSTACK_CALL;

	XEvent evt;

	while (XPending(m_display) > 0)
	{
		XNextEvent(m_display, &evt);

		/* draw or redraw the window */
		if (evt.type == Expose || evt.type == GraphicsExpose)
		{
			auto listener = m_listener.lock();

			if (listener != null)
			{
				listener->OnRedraw(this);
			}
			continue;
		}
		if (evt.type == VisibilityNotify)
		{
			mst::printf("state: %d\n", evt.xvisibility.state);
		}
		if (evt.type == KeyPress || evt.type == KeyRelease)
		{
			auto listener = m_listener.lock();

			if (listener == null)
				continue;

			if (evt.type == KeyRelease && XEventsQueued(m_display, QueuedAfterReading))
			{
				XEvent nev;
				XPeekEvent(m_display, &nev);

				if (nev.type == KeyPress && nev.xkey.time == evt.xkey.time &&
					nev.xkey.keycode == evt.xkey.keycode)
				{
					/* Key wasn't actually released */
					continue;
				}
			}

			handleKeyEvent(&evt, listener);
		}
		if (evt.type == MapNotify)
		{
			m_shown = true;
		}
		if (evt.type == UnmapNotify)
		{
			m_shown = false;
		}
		if (evt.type == ClientMessage)
		{
			if ((Atom)evt.xclient.data.l[0] == m_deleteWindowAtom)
			{
				m_shown = false;
			}
		}
	}
}

bool XorgWindow::isShown() const
{
	COMMON_CALLSTACK_CALL;

	return m_shown;
}

string XorgWindow::getName() const
{
	COMMON_CALLSTACK_CALL;

	return m_name;
}

int XorgWindow::getWidth() const
{
	COMMON_CALLSTACK_CALL;

	Common::fatalError("Not implemented");
	return 0;
}

int XorgWindow::getHeight() const
{
	COMMON_CALLSTACK_CALL;

	Common::fatalError("Not implemented");
	return 0;
}

Rect XorgWindow::getRect() const
{
	COMMON_CALLSTACK_CALL;

	Common::fatalError("Not implemented");
	return Rect();
}

Rect XorgWindow::getClientRect() const
{
	COMMON_CALLSTACK_CALL;

	Common::fatalError("Not implemented");
	return Rect();
}

void XorgWindow::toForeground()
{
	COMMON_CALLSTACK_CALL;

	Common::fatalError("Not implemented");
}

::Display* XorgWindow::getDisplay() const
{
	return m_display;
}

::Window XorgWindow::getWindow() const
{
	return m_window;
}

void* XorgWindow::asImpl(uint64_t typeHash) const
{
	COMMON_CALLSTACK_CALL;

	switch (typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(IWindow);
		CHECK_TYPE_AND_RETURN(IXorgWindow);
		CHECK_TYPE_AND_RETURN(XorgWindow);
	default:
		return nullptr;
	}
}

void XorgWindow::handleKeyEvent(XEvent* evt, const shared_ptr<ClientInterfaces::IWindowEventListener>& listener)
{
	auto sym = XkbKeycodeToKeysym(m_display, evt->xkey.keycode, 0, (evt->xkey.state & ShiftMask) != 0 ? 1 : 0);

	auto keyCode = EnumConverter::toKeyCodes(sym);

	mst::flag<KeyModifiers> modifiers;

	if ((evt->xkey.state & ControlMask) != 0)
		modifiers.enable(KeyModifiers::Ctrl);

	if ((evt->xkey.state & ShiftMask) != 0)
		modifiers.enable(KeyModifiers::Shift);

	if ((evt->xkey.state & LockMask) != 0)
		modifiers.enable(KeyModifiers::CapsLock);

	if (evt->type == KeyPress)
	{
		listener->OnKeyDown(this, keyCode, modifiers);
	}
	else
	{
		// On Keyup
	}
}
