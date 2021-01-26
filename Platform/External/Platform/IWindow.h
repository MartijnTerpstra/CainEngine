#pragma once

namespace CainEngine {
namespace Platform {

/**
	Sunmmary:
		Interface to a window
*/
class IWindow : public Common::BaseObject
{
	COMMON_DECLARE_INTERFACE(IWindow);

public:
	// Main functionality

	/**
		Summary:
			Shows the window
	*/
	virtual void Show() = 0;

	/**
		Summary:
			Redraws the window
	*/
	virtual void Redraw() = 0;

	/**
		Summary:
			Maximizes the window
	*/
	virtual void Maximize() = 0;

	/**
		Summary:
			Minimizes the window
	*/
	virtual void Minimize() = 0;

	/**
		Summary:
			Closes the window
	*/
	virtual void Close() = 0;



	/**
		Summary:
			Handles events

		Details:
			Events are sent to the window event listener attached at creation
	*/
	virtual void HandleEvents() = 0;

	/**
		Summary:
			Returns whether the window is shown
	*/
	virtual bool IsShown() const = 0;

	/**
		Summary:
			Returns the window's name
	*/
	virtual string GetName() const = 0;

	/**
		Summary:
			Returns the window's width
	*/
	virtual int GetWidth() const = 0;

	/**
		Summary:
			Returns the window's height
	*/
	virtual int GetHeight() const = 0;

	/**
		Summary:
			Returns the window's rectangle on the monitor
	*/
	virtual Rect GetRect() const = 0;

	/**
		Summary:
			Returns the window's inside rectangle
	*/
	virtual Rect GetClientRect() const = 0;

	/**
		Summary:
			Places the window on  the foreground of the monitor
	*/
	virtual void ToForeground() = 0;

}; // class IWindow

inline IWindow::~IWindow() = default;

}; // namespace Platform
}; // namespace CainEngine