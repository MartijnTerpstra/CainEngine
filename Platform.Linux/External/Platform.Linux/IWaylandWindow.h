#pragma once

namespace CainEngine {
namespace Platform {
namespace Linux {

/**
	Summary:
		Interface to a Wayland window
*/
class IWaylandWindow : public IWindow
{
	COMMON_DECLARE_INTERFACE(IWaylandWindow);

public:
	// Main functionality

	virtual ::wl_display* getDisplay() const = 0;
	virtual ::wl_surface* getSurface() const = 0;

}; // class IWaylandWindow

inline IWaylandWindow::~IWaylandWindow() = default;

}; // namespace Linux
}; // namespace Platform
}; // namespace CainEngine
