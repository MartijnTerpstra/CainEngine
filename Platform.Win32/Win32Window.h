#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

class Win32Window final : public Win32::IWin32Window
{
	friend class RefPtr<Win32Window>;
private:
	// ctor & dtor

	Win32Window(HWND hwnd, string name, const shared_ptr<ClientInterfaces::IWindowEventListener>& listener, ClientInterfaces::IWindowEventListener* listenerPointer);
	~Win32Window();

	COMMON_DECLARE_NON_COPY(Win32Window);

public:
	// Creation

	static RefPtr<IWindow> CreateNewWindow(const string& name, const uint2& size, WindowType type, flag<WindowFlags> flags, const shared_ptr<ClientInterfaces::IWindowEventListener>& listener, ClientInterfaces::IWindowEventListener* listenerPointer);

	static RefPtr<IWindow> GetConsole();

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
	// IWin32Window overrides

	HWND GetHwnd() const override;

private:
	// BaseObject overrides

	virtual void * _As(uint64_t) const override;

private:
	// Internal functionality

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT OnWndProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	// Member variables

	bool m_minimized;

	const HWND m_hwnd;
	const string m_name;
	const weak_ptr<ClientInterfaces::IWindowEventListener> m_eventListener;
	ClientInterfaces::IWindowEventListener* const m_eventListenerPointer;

}; // namespace Win32Window

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine