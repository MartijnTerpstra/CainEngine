#pragma once

namespace CainEngine {
namespace Platform {
namespace Xorg {

/**
	Summary:
		Interface to an Xorg window
*/
class IXorgWindow : public IWindow
{
	COMMON_DECLARE_INTERFACE(IXorgWindow);

public:
	// Main functionality

	virtual ::Display* GetDisplay() const = 0;
	virtual ::Window GetWindow() const = 0;

}; // class IXorgWindow

inline IXorgWindow::~IXorgWindow() = default;

}; // namespace Xorg
}; // namespace Platform
}; // namespace CainEngine
