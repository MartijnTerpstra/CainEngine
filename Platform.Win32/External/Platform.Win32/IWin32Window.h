#pragma once

namespace CainEngine {
namespace Platform {
namespace Win32 {

/**
	Summary:
		Interface to a windows window
*/
class IWin32Window : public IWindow
{
	COMMON_DECLARE_INTERFACE(IWin32Window);

public:
	// Main functionality

	virtual HWND GetHwnd() const = 0;

}; // class IWin32Window

inline IWin32Window::~IWin32Window() = default;

}; // namespace Win32
}; // namespace Platform
}; // namespace CainEngine