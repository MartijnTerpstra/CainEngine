#pragma once

namespace CainEngine {
namespace Platform {
namespace Linux {

/**
	Summary:
		Interface to a Linux window
*/
class ILinuxWindow : public IWindow, public Common::Castable<ILinuxWindow>
{
	COMMON_DECLARE_INTERFACE(ILinuxWindow);

public:
	// Main functionality

	virtual ::Display* GetDisplay() const = 0;
	virtual ::Window GetWindow() const = 0;

}; // class ILinuxWindow

inline ILinuxWindow::~ILinuxWindow() = default;

}; // namespace Linux
}; // namespace Platform
}; // namespace CainEngine