#pragma once

namespace CainEngine {
namespace Platform {
namespace ClientInterfaces {

class IWindowEventListener
{
	COMMON_DECLARE_INTERFACE(IWindowEventListener);

public:
	// Main functionality

	/**
		Summary:
			A window resize event callback

		Params:
			- newSize: the new size of the window
	*/
	virtual void onResize(IWindow* window, const uint2& newSize)
	{
		_MST_UNUSED(window);
		_MST_UNUSED(newSize);
	};

	/**
		Summary:
			A minimize event callback
	*/
	virtual void onMinimize(IWindow* window)
	{
		_MST_UNUSED(window);
	};

	/**
		Summary:
			A minimize event callback
	*/
	virtual void onMaximize(IWindow* window)
	{
		_MST_UNUSED(window);
	};

	/**
		Summary:
			A redraw event callback
	*/
	virtual void onRedraw(IWindow* window)
	{
		_MST_UNUSED(window);
	};

	/**
		Summary:
			A key down event callback
	*/
	virtual void onKeyDown(IWindow* window, KeyCodes keyCode, flag<KeyModifiers> modifiers)
	{
		_MST_UNUSED(window);
		_MST_UNUSED(keyCode);
		_MST_UNUSED(modifiers);
	};

}; // class IWindowEventListener

inline IWindowEventListener::~IWindowEventListener() = default;

}; // namespace ClientInterfaces
}; // namespace Platform
}; // namespace CainEngine