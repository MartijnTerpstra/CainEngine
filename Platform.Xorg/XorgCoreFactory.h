#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

/*
	Summary:
		Xorg implementation of the IPlatformFactory
*/
class XorgCoreFactory final : public ICoreFactory
{
	COMMON_DECLARE_NON_COPY(XorgCoreFactory);
	friend class Common::RefPtr<XorgCoreFactory>;

private:
	// ctor & dtor

	XorgCoreFactory();
	~XorgCoreFactory();

public:
	// IPlatformFactory overrides

	string GetPlatformName() const override;

	vector<RefPtr<IProcess>> GetProcesses(const string& name) override;

	RefPtr<IProcess> GetProcess(uint32_t id) override;

	RefPtr<IProcess> GetCurrentProcess() override;

	RefPtr<IProcess> CreateNewProcess(const string& path) override;

	RefPtr<IProcess> CreateNewProcess(const string& path, const string& commandLine) override;

	RefPtr<IProcess> CreateNewProcess(
		const string& path, const string& commandLine, const string& workingDirectory) override;

	vector<RefPtr<IMonitor>> GetMonitors() override;

	RefPtr<IMonitor> GetMainMonitor() override;

	RefPtr<IWindow> CreateNewWindow(const string& name, const uint2& size, WindowType type,
		flag<WindowFlags> flags,
		const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener) override;

	RefPtr<IWindow> CreateNewWindow(const string& name, const uint2& size, WindowType type,
		flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener) override;

	RefPtr<IWindow> GetConsoleWindow() override;

private:
	// ICastable overrides

	void* _As(uint64_t) const override;

private:
	// Member variables

}; // class XorgCoreFactory

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine
