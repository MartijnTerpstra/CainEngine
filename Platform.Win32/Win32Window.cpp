#include "Precomp.h"

#include "Win32Window.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

Win32Window::Win32Window(HWND hwnd, std::string name,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
	ClientInterfaces::IWindowEventListener* listenerPointer)
	: m_hwnd(hwnd)
	, m_name(std::move(name))
	, m_minimized(false)
	, m_eventListener(listener)
	, m_eventListenerPointer(listenerPointer)
{ }

Win32Window::~Win32Window()
{
	auto thisPtr = (std::weak_ptr<Win32Window>*)GetWindowLongPtrA(m_hwnd, GWLP_USERDATA);

	SetWindowLongPtrA(m_hwnd, GWLP_USERDATA, 0);

	if(thisPtr)
		delete thisPtr;
}

RefPtr<IWindow> Win32Window::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
	ClientInterfaces::IWindowEventListener* listenerPointer)
{
	HINSTANCE instance = GetModuleHandleA(nullptr);

	WNDCLASSA wc;
	if(!GetClassInfoA(instance, name.c_str(), &wc))
	{
		// create window class
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = &Win32Window::wndProc;
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

	switch(type)
	{
	case Platform::WindowType::Sizable:
		style = WS_OVERLAPPEDWINDOW;
		break;
	case Platform::WindowType::Borderless:
		style = WS_OVERLAPPED;
		break;
	case Platform::WindowType::FullscreenBorderless:
		Common::fatalError("Not implemented: Platform::WindowType::Fullscreen");
		break;
	default:
		Common::fatalError("WindowsWindow::CreateNewWindow(): corrupted value: type");
	}

	if(flags.is_enabled(Platform::WindowFlags::NoMinimizeButton))
	{
		style &= ~WS_MINIMIZEBOX;
	}

	if(flags.is_enabled(Platform::WindowFlags::NoMaximizeButton))
	{
		style &= ~WS_MAXIMIZEBOX;
	}

	if(flags.is_enabled(Platform::WindowFlags::NoCloseButton))
	{
		style &= ~WS_SYSMENU;
	}

	HWND hwnd = CreateWindowExA(WS_EX_APPWINDOW, name.c_str(), name.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, size.x, size.y, nullptr, nullptr, instance, nullptr);

	if(!hwnd)
	{
		return nullptr;
	}

	auto retval = Common::RefPtr<Win32Window>::create(hwnd, name, listener, listenerPointer);

	SetWindowLongPtrA(retval->m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(retval.get()));

	return retval;
}

RefPtr<IWindow> Win32Window::getConsole()
{
	auto hwnd = GetConsoleWindow();

	if(hwnd == nullptr)
		return nullptr;

	auto retval = Common::RefPtr<Win32Window>::create(hwnd, "Console", nullptr, nullptr);

	SetWindowLongPtrA(retval->m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(retval.get()));

	return retval;
}

void Win32Window::show()
{
	ShowWindow(m_hwnd, SW_NORMAL);
}

void Win32Window::redraw()
{
	RedrawWindow(m_hwnd, nullptr, nullptr, RDW_INTERNALPAINT);
}

void Win32Window::maximize()
{
	ShowWindow(m_hwnd, SW_MAXIMIZE);
}

void Win32Window::close()
{
	DestroyWindow(m_hwnd);
}

void Win32Window::minimize()
{
	ShowWindow(m_hwnd, SW_MINIMIZE);
}

void Win32Window::handleEvents()
{
	MSG msg = {};

	while(1)
	{
		BOOL result = PeekMessageA(&msg, m_hwnd, 0, 0, PM_REMOVE);

		if(result == FALSE || result == -1)
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

bool Win32Window::isShown() const
{
	return (IsWindowVisible(m_hwnd) != FALSE);
}

std::string Win32Window::getName() const
{
	return m_name;
}

int Win32Window::getWidth() const
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);

	return rect.right - rect.left;
}

int Win32Window::getHeight() const
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);

	return rect.bottom - rect.top;
}

Rect Win32Window::getRect() const
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);

	return Rect(rect.left, rect.top, rect.right, rect.bottom);
}

Rect Win32Window::getClientRect() const
{
	RECT rect;
	::GetClientRect(m_hwnd, &rect);

	return Rect(rect.left, rect.top, rect.right, rect.bottom);
}

void Win32Window::toForeground()
{
	SetForegroundWindow(m_hwnd);
}

HWND Win32Window::getHwnd() const
{
	return m_hwnd;
}

void* Win32Window::asImpl(uint64_t typeHash) const
{
	switch(typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(IWindow);
		CHECK_TYPE_AND_RETURN(IWin32Window);
		CHECK_TYPE_AND_RETURN(Win32Window);
	default:
		return nullptr;
	}
}