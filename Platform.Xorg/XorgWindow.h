#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

class XorgWindow final : public Xorg::IXorgWindow
{
	friend class Common::RefPtr<XorgWindow>;

private:
	// ctor & dtor

	XorgWindow(Display* display, ::Window window, string name,
		const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
		ClientInterfaces::IWindowEventListener* listenerPointer);
	~XorgWindow();

	COMMON_DECLARE_NON_COPY(XorgWindow);

public:
	// Creation

	static RefPtr<IWindow> CreateNewWindow(const string& name, const uint2& size, WindowType type,
		flag<WindowFlags> flags,
		const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
		ClientInterfaces::IWindowEventListener* listenerPointer);

public:
	// IWindow overrides

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
	// IXorgWindow overrides

	::Display* GetDisplay() const override;

	::Window GetWindow() const override;

private:
	// BaseObject overrides

	void* _As(uint64_t) const override;

private:
	// Internal functionality

	void HandleKeyEvent(XEvent* evt, const shared_ptr<ClientInterfaces::IWindowEventListener>& listener);

private:
	// Member variables

	::Display *const m_display;
	const ::Window m_window;
	const string m_name;
	const std::weak_ptr<ClientInterfaces::IWindowEventListener> m_listener;
	ClientInterfaces::IWindowEventListener* const m_listenerPointer;
	Atom m_deleteWindowAtom = 0;
	bool m_shown = false;

}; // class XorgWindow

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine
