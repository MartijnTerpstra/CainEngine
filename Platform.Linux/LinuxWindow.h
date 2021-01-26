#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

class LinuxWindow final : public Linux::ILinuxWindow, public Common::Castable<LinuxWindow>
{
public:
	LinuxWindow(Display* display, ::Window window, string name, const weak_ptr<ClientInterfaces::IWindowEventListener>& listener);
	~LinuxWindow();

public:
	// Creation

	static shared_ptr<IWindow> CreateNewWindow(const string& name, const uint2& size, WindowType type, flag<WindowFlags> flags, const weak_ptr<ClientInterfaces::IWindowEventListener>& listener);

public:
	// IWindow implementations

	void Show() override;

	void Redraw() override;

	void Maximize() override;

	void Minimize() override;

	void Close() override;

	void HandleEvents() override;

	bool IsShown() const override;

	string GetName() const override;

	int GetWidth() const override;

	int GetHeight() const override;

	Rect GetRect() const override;

	Rect GetClientRect() const override;

	void ToForeground() override;

public:
	// ILinuxWindow implementations

	::Display* GetDisplay() const override;

	::Window GetWindow() const override;

private:
	// Internal functionality

	void HandleKeyEvent(XEvent* evt, const shared_ptr<ClientInterfaces::IWindowEventListener>& listener);

private:
	// Member variables

	::Display *const m_display;
	const ::Window m_window;
	const string m_name;
	const weak_ptr<ClientInterfaces::IWindowEventListener> m_listener;
	Atom m_deleteWindowAtom = 0;
	bool m_shown = false;

}; // class EnumConverter

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine