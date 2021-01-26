#include "Precomp.h"

#include "Win32Window.h"

#include "EnumConverter.h"

using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

LRESULT Win32Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	COMMON_CALLSTACK_CALL;

	auto backPtr = reinterpret_cast<Win32Window*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));

	if (backPtr != nullptr)
	{
		return backPtr->OnWndProc(message, wParam, lParam);
	}
	
	return DefWindowProcA(hwnd, message, wParam, lParam);
}

LRESULT Win32Window::OnWndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	COMMON_CALLSTACK_CALL;

	switch (message)
	{
	case WM_SIZE:
	{
		auto listener = m_eventListener.lock();
		if (listener)
		{
			auto newSize = uint2(LOWORD(lParam), HIWORD(lParam));

			if (m_minimized)
			{
				if (newSize != uint2::zero)
				{
					m_minimized = false;
					listener->OnMaximize(this);
				}
			}
			else
			{
				if (newSize == uint2::zero)
				{
					m_minimized = true;
					listener->OnMinimize(this);
				}
				else
				{
					listener->OnResize(this, newSize);
				}
			}
		}
		break;
	}
	case WM_PAINT:
	{
		auto listener = m_eventListener.lock();
		if (listener)
			listener->OnRedraw(this);
		break;
	}
	case WM_KEYDOWN:
	{
		auto listener = m_eventListener.lock();

		KeyCodes keyCode = EnumConverter::ToKeyCodes(wParam);

		mst::flag<KeyModifiers> modifiers;

		if (keyCode != KeyCodes::Shift && keyCode != KeyCodes::LeftShift && keyCode != KeyCodes::RightShift && GetKeyState(VK_SHIFT) & 0x80)
		{
			modifiers.enable(KeyModifiers::Shift);
		}

		if (keyCode != KeyCodes::LeftCtrl && keyCode != KeyCodes::RightCtrl && GetKeyState(VK_CONTROL) & 0x80)
		{
			modifiers.enable(KeyModifiers::Ctrl);
		}

		if (keyCode != KeyCodes::LeftAlt && keyCode != KeyCodes::RightAlt && GetKeyState(VK_MENU) & 0x80)
		{
			modifiers.enable(KeyModifiers::Alt);
		}

		if (GetKeyState(VK_CAPITAL) & 0x1)
		{
			modifiers.enable(KeyModifiers::CapsLock);
		}

		if (GetKeyState(VK_NUMLOCK) & 0x1)
		{
			modifiers.enable(KeyModifiers::NumLock);
		}

		if (listener)
			listener->OnKeyDown(this, keyCode, modifiers);
		break;
	}
	}

	return DefWindowProcA(m_hwnd, message, wParam, lParam);
}