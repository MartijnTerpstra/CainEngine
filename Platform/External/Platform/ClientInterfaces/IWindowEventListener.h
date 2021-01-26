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
	virtual void OnResize(IWindow* window, const uint2& newSize) { _MST_UNUSED(window); _MST_UNUSED(newSize); };

	/**
		Summary:
			A minimize event callback
	*/
	virtual void OnMinimize(IWindow* window) { _MST_UNUSED(window); };

	/**
		Summary:
			A minimize event callback
	*/
	virtual void OnMaximize(IWindow* window) { _MST_UNUSED(window); };

	/**
		Summary:
			A redraw event callback
	*/
	virtual void OnRedraw(IWindow* window) { _MST_UNUSED(window); };

	/**
		Summary:
			A key down event callback
	*/
	virtual void OnKeyDown(IWindow* window, KeyCodes keyCode, flag<KeyModifiers> modifiers) { _MST_UNUSED(window); _MST_UNUSED(keyCode); _MST_UNUSED(modifiers); };

}; // class IWindowEventListener

inline IWindowEventListener::~IWindowEventListener() = default;

}; // namespace ClientInterfaces
}; // namespace Platform
}; // namespace CainEngine