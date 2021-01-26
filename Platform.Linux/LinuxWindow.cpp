#include "Precomp.h"

#include "LinuxWindow.h"

#include "EnumConverter.h"

#include <X11/XKBlib.h>
#include <X11/Xutil.h>

using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

LinuxWindow::LinuxWindow(Display* display, ::Window window, string name, const weak_ptr<ClientInterfaces::IWindowEventListener>& listener)
	: m_display(display),
	m_window(window),
	m_name(move(name)),
	m_listener(listener)
{
	COMMON_CALLSTACK_CALL;
}

LinuxWindow::~LinuxWindow()
{
	COMMON_CALLSTACK_CALL;

	XDestroyWindow(m_display, m_window);

	XCloseDisplay(m_display);
}

shared_ptr<IWindow> LinuxWindow::CreateNewWindow(const string& name, const uint2& size, WindowType type, flag<WindowFlags> flags, const weak_ptr<ClientInterfaces::IWindowEventListener>& listener)
{
	COMMON_CALLSTACK_CALL;

	Display* display = XOpenDisplay(":0.0");
	if (display == null)
	{
		Common::FatalError("LinuxWindow::CreateNewWindow(): XOpenDisplay failed");
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

	return make_shared<LinuxWindow>(display, window, name, listener);
}

void LinuxWindow::Show()
{
	COMMON_CALLSTACK_CALL;

	m_shown = true;

	XMapWindow(m_display, m_window);
	XFlush(m_display);

	m_deleteWindowAtom =
		XInternAtom(m_display, "WM_DELETE_WINDOW", False);
}

void LinuxWindow::Redraw()
{
	COMMON_CALLSTACK_CALL;

	XEvent evt = {};

	evt.type = Expose;

	auto res = XSendEvent(m_display, m_window, True, ExposureMask, &evt);

	int gege = 4;
}

void LinuxWindow::Maximize()
{
	COMMON_CALLSTACK_CALL;

	Common::FatalError("Not implemented");
}

void LinuxWindow::Minimize()
{
	COMMON_CALLSTACK_CALL;

	Common::FatalError("Not implemented");
}

void LinuxWindow::Close()
{
	COMMON_CALLSTACK_CALL;

	m_shown = false;
	XDestroyWindow(m_display, m_window);
}

void LinuxWindow::HandleEvents()
{
	COMMON_CALLSTACK_CALL;

	XEvent evt;

	while (XPending(m_display) > 0)
	{
		XNextEvent(m_display, &evt);

		/* draw or redraw the window */
		if (evt.type == Expose || evt.type == GraphicsExpose)
		{
			/*auto os = mst::platform::full_name();

			XFillRectangle(m_display, window, DefaultGC(display, s), 20, 20, 10, 10);
			XDrawString(display, window, DefaultGC(display, s), 50, 50, os.c_str(), os.length());*/
			auto listener = m_listener.lock();

			if (listener != null)
			{
				listener->OnRedraw(shared_this_ptr);
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
					/* Key wasn’t actually released */
					continue;
				}
			}

			HandleKeyEvent(&evt, listener);
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

bool LinuxWindow::IsShown() const
{
	COMMON_CALLSTACK_CALL;

	return m_shown;
}

string LinuxWindow::GetName() const
{
	COMMON_CALLSTACK_CALL;

	return m_name;
}

int LinuxWindow::GetWidth() const
{
	COMMON_CALLSTACK_CALL;

	Common::FatalError("Not implemented");
	return 0;
}

int LinuxWindow::GetHeight() const
{
	COMMON_CALLSTACK_CALL;

	Common::FatalError("Not implemented");
	return 0;
}

Rect LinuxWindow::GetRect() const
{
	COMMON_CALLSTACK_CALL;

	Common::FatalError("Not implemented");
	return Rect();
}

Rect LinuxWindow::GetClientRect() const
{
	COMMON_CALLSTACK_CALL;

	Common::FatalError("Not implemented");
	return Rect();
}

void LinuxWindow::ToForeground()
{
	COMMON_CALLSTACK_CALL;

	Common::FatalError("Not implemented");
}

::Display* LinuxWindow::GetDisplay() const
{
	return m_display;
}

::Window LinuxWindow::GetWindow() const
{
	return m_window;
}

void LinuxWindow::HandleKeyEvent(XEvent* evt, const shared_ptr<ClientInterfaces::IWindowEventListener>& listener)
{

	auto sym = XkbKeycodeToKeysym(m_display, evt->xkey.keycode, 0, (evt->xkey.state & ShiftMask) != 0 ? 1 : 0);

	auto keyCode = EnumConverter::ToKeyCodes(sym);

	mst::flag<KeyModifiers> modifiers;

	if ((evt->xkey.state & ControlMask) != 0)
		modifiers.enable(KeyModifiers::Ctrl);

	if ((evt->xkey.state & ShiftMask) != 0)
		modifiers.enable(KeyModifiers::Shift);

	if ((evt->xkey.state & LockMask) != 0)
		modifiers.enable(KeyModifiers::CapsLock);

	if (evt->type == KeyPress)
	{
		listener->OnKeyDown(shared_this_ptr, keyCode, modifiers);
	}
	else
	{
		// On Keyup
	}
}