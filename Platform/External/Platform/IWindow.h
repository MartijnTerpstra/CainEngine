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
	virtual void show() = 0;

	/**
		Summary:
			Redraws the window
	*/
	virtual void redraw() = 0;

	/**
		Summary:
			Maximizes the window
	*/
	virtual void maximize() = 0;

	/**
		Summary:
			Minimizes the window
	*/
	virtual void minimize() = 0;

	/**
		Summary:
			Closes the window
	*/
	virtual void close() = 0;



	/**
		Summary:
			Handles events

		Details:
			Events are sent to the window event listener attached at creation
	*/
	virtual void handleEvents() = 0;

	/**
		Summary:
			Returns whether the window is shown
	*/
	virtual bool isShown() const = 0;

	/**
		Summary:
			Returns the window's name
	*/
	virtual std::string getName() const = 0;

	/**
		Summary:
			Returns the window's width
	*/
	virtual int getWidth() const = 0;

	/**
		Summary:
			Returns the window's height
	*/
	virtual int getHeight() const = 0;

	/**
		Summary:
			Returns the window's rectangle on the monitor
	*/
	virtual Rect getRect() const = 0;

	/**
		Summary:
			Returns the window's inside rectangle
	*/
	virtual Rect getClientRect() const = 0;

	/**
		Summary:
			Places the window on  the foreground of the monitor
	*/
	virtual void toForeground() = 0;

}; // class IWindow

inline IWindow::~IWindow() = default;

}; // namespace Platform
}; // namespace CainEngine