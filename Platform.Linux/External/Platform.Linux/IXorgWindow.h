#pragma once

namespace CainEngine {
namespace Platform {
namespace Linux {

/**
	Summary:
		Interface to an Xorg window
*/
class IXorgWindow : public IWindow
{
	COMMON_DECLARE_INTERFACE(IXorgWindow);

public:
	// Main functionality

	virtual ::Display* getDisplay() const = 0;
	virtual ::Window getWindow() const = 0;

}; // class IXorgWindow

inline IXorgWindow::~IXorgWindow() = default;

}; // namespace Linux
}; // namespace Platform
}; // namespace CainEngine
