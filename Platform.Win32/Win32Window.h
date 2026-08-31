#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

class Win32Window final : public Win32::IWin32Window
{
	friend class RefPtr<Win32Window>;
private:
	// ctor & dtor

	Win32Window(HWND hwnd, std::string name, const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener, ClientInterfaces::IWindowEventListener* listenerPointer);
	~Win32Window();

	COMMON_DECLARE_NON_COPY(Win32Window);

public:
	// Creation

	static RefPtr<IWindow> createNewWindow(const std::string& name, const uint2& size, WindowType type, flag<WindowFlags> flags, const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener, ClientInterfaces::IWindowEventListener* listenerPointer);

	static RefPtr<IWindow> getConsole();

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
	// IWin32Window overrides

	HWND getHwnd() const override;

private:
	// BaseObject overrides

	virtual void * asImpl(uint64_t) const override;

private:
	// Internal functionality

	static LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT onWndProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	// Member variables

	bool m_minimized;

	const HWND m_hwnd;
	const std::string m_name;
	const std::weak_ptr<ClientInterfaces::IWindowEventListener> m_eventListener;
	ClientInterfaces::IWindowEventListener* const m_eventListenerPointer;

}; // namespace Win32Window

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine