#include "Precomp.h"

#include "Win32Window.h"

using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

Win32Window::Win32Window(HWND hwnd, string name, const shared_ptr<ClientInterfaces::IWindowEventListener>& listener, ClientInterfaces::IWindowEventListener* listenerPointer)
	: m_hwnd(hwnd), m_name(std::move(name))
	, m_minimized(false)
	, m_eventListener(listener)
	, m_eventListenerPointer(listenerPointer)
{
	COMMON_CALLSTACK_CALL;
}

Win32Window::~Win32Window()
{
	COMMON_CALLSTACK_CALL;

	auto thisPtr = (weak_ptr<Win32Window>*)GetWindowLongPtrA(m_hwnd, GWLP_USERDATA);

	SetWindowLongPtrA(m_hwnd, GWLP_USERDATA, 0);

	if (thisPtr)
		delete thisPtr;
}

RefPtr<IWindow> Win32Window::CreateNewWindow(const string& name, const uint2& size, WindowType type, flag<WindowFlags> flags, const shared_ptr<ClientInterfaces::IWindowEventListener>& listener, ClientInterfaces::IWindowEventListener* listenerPointer)
{
	COMMON_CALLSTACK_CALL;

	HINSTANCE instance = GetModuleHandleA(nullptr);

	WNDCLASSA wc;
	if (!GetClassInfoA(instance, name.c_str(), &wc))
	{
		// create window class
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = &Win32Window::WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = instance;
		wc.hIcon = nullptr;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = name.c_str();

		// register class
		RegisterClassA(&wc);
	}

	DWORD style;

	switch (type)
	{
	case Platform::WindowType::Sizable:
		style = WS_OVERLAPPEDWINDOW;
		break;
	case Platform::WindowType::Borderless:
		style = WS_OVERLAPPED;
		break;
	case Platform::WindowType::FullscreenBorderless:
		Common::FatalError("Not implemented: Platform::WindowType::Fullscreen");
		break;
	default:
		Common::FatalError("WindowsWindow::CreateNewWindow(): corrupted value: type");
	}

	if (flags.is_enabled(Platform::WindowFlags::NoMinimizeButton))
	{
		style &= ~WS_MINIMIZEBOX;
	}

	if (flags.is_enabled(Platform::WindowFlags::NoMaximizeButton))
	{
		style &= ~WS_MAXIMIZEBOX;
	}

	if (flags.is_enabled(Platform::WindowFlags::NoCloseButton))
	{
		style &= ~WS_SYSMENU;
	}

	HWND hwnd = CreateWindowExA(WS_EX_APPWINDOW, name.c_str(), name.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, size.x, size.y, nullptr, nullptr, instance, nullptr);

	if (!hwnd)
	{
		return nullptr;
	}

	auto retval = Common::RefPtr<Win32Window>::Create(hwnd, name, listener, listenerPointer);

	SetWindowLongPtrA(retval->m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(retval.Get()));

	return retval;
}

RefPtr<IWindow> Win32Window::GetConsole()
{
	COMMON_CALLSTACK_CALL;

	auto hwnd = GetConsoleWindow();

	if (hwnd == nullptr)
		return nullptr;

	auto retval = Common::RefPtr<Win32Window>::Create(hwnd, "Console", nullptr, nullptr);

	SetWindowLongPtrA(retval->m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(retval.Get()));

	return retval;
}

void Win32Window::Show()
{
	COMMON_CALLSTACK_CALL;

	ShowWindow(m_hwnd, SW_NORMAL);
}

void Win32Window::Redraw()
{
	COMMON_CALLSTACK_CALL;

	RedrawWindow(m_hwnd, nullptr, nullptr, RDW_INTERNALPAINT);
}

void Win32Window::Maximize()
{
	COMMON_CALLSTACK_CALL;

	ShowWindow(m_hwnd, SW_MAXIMIZE);
}

void Win32Window::Close()
{
	COMMON_CALLSTACK_CALL;

	DestroyWindow(m_hwnd);
}

void Win32Window::Minimize()
{
	COMMON_CALLSTACK_CALL;

	ShowWindow(m_hwnd, SW_MINIMIZE);
}

void Win32Window::HandleEvents()
{
	COMMON_CALLSTACK_CALL;

	MSG msg = { };

	while (1)
	{
		BOOL result = PeekMessageA(&msg, m_hwnd, 0, 0, PM_REMOVE);

		if (result == FALSE || result == -1)
		{
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
}

bool Win32Window::IsShown() const
{
	COMMON_CALLSTACK_CALL;

	return (IsWindowVisible(m_hwnd) != FALSE);
}

string Win32Window::GetName() const
{
	COMMON_CALLSTACK_CALL;

	return m_name;
}

int Win32Window::GetWidth() const
{
	COMMON_CALLSTACK_CALL;

	RECT rect;
	GetWindowRect(m_hwnd, &rect);

	return rect.right - rect.left;
}

int Win32Window::GetHeight() const
{
	COMMON_CALLSTACK_CALL;

	RECT rect;
	GetWindowRect(m_hwnd, &rect);

	return rect.bottom - rect.top;
}

Rect Win32Window::GetRect() const
{
	COMMON_CALLSTACK_CALL;

	RECT rect;
	GetWindowRect(m_hwnd, &rect);

	return Rect(rect.left, rect.top, rect.right, rect.bottom);
}

Rect Win32Window::GetClientRect() const
{
	COMMON_CALLSTACK_CALL;

	RECT rect;
	::GetClientRect(m_hwnd, &rect);

	return Rect(rect.left, rect.top, rect.right, rect.bottom);
}

void Win32Window::ToForeground()
{
	COMMON_CALLSTACK_CALL;

	SetForegroundWindow(m_hwnd);
}

HWND Win32Window::GetHwnd() const
{
	COMMON_CALLSTACK_CALL;

	return m_hwnd;
}

void* Win32Window::_As(uint64_t typeHash) const
{
	COMMON_CALLSTACK_CALL;

	switch (typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(IWindow);
		CHECK_TYPE_AND_RETURN(IWin32Window);
		CHECK_TYPE_AND_RETURN(Win32Window);
	default:
		Common::FatalError("Invalid cast");
	}
}