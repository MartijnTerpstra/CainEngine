#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

class XorgWindow final : public Xorg::IXorgWindow
{
	friend class Common::RefPtr<XorgWindow>;

private:
	// ctor & dtor

	XorgWindow(Display* display, ::Window window, std::string name,
		const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
		ClientInterfaces::IWindowEventListener* listenerPointer);
	~XorgWindow();

	COMMON_DECLARE_NON_COPY(XorgWindow);

public:
	// Creation

	static RefPtr<IWindow> createNewWindow(const std::string& name, const uint2& size, WindowType type,
		flag<WindowFlags> flags,
		const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener,
		ClientInterfaces::IWindowEventListener* listenerPointer);

public:
	// IWindow overrides

	void show() override;

	void redraw() override;

	void maximize() override;

	void minimize() override;

	void close() override;

	void handleEvents() override;

	bool isShown() const override;

	std::string getName() const override;

	int getWidth() const override;

	int getHeight() const override;

	Rect getRect() const override;

	Rect getClientRect() const override;

	void toForeground() override;

public:
	// IXorgWindow overrides

	::Display* getDisplay() const override;

	::Window getWindow() const override;

private:
	// BaseObject overrides

	void* asImpl(uint64_t) const override;

private:
	// Internal functionality

	void handleKeyEvent(XEvent* evt, const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener);

private:
	// Member variables

	::Display *const m_display;
	const ::Window m_window;
	const std::string m_name;
	const std::weak_ptr<ClientInterfaces::IWindowEventListener> m_listener;
	ClientInterfaces::IWindowEventListener* const m_listenerPointer;
	Atom m_deleteWindowAtom = 0;
	bool m_shown = false;

}; // class XorgWindow

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine
