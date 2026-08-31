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

	string getPlatformName() const override;

	vector<RefPtr<IProcess>> getProcesses(const string& name) override;

	RefPtr<IProcess> getProcess(uint32_t id) override;

	RefPtr<IProcess> getCurrentProcess() override;

	RefPtr<IProcess> createNewProcess(const string& path) override;

	RefPtr<IProcess> createNewProcess(const string& path, const string& commandLine) override;

	RefPtr<IProcess> createNewProcess(
		const string& path, const string& commandLine, const string& workingDirectory) override;

	vector<RefPtr<IMonitor>> getMonitors() override;

	RefPtr<IMonitor> getMainMonitor() override;

	RefPtr<IWindow> createNewWindow(const string& name, const uint2& size, WindowType type,
		flag<WindowFlags> flags,
		const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener) override;

	RefPtr<IWindow> createNewWindow(const string& name, const uint2& size, WindowType type,
		flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener) override;

	RefPtr<IWindow> getConsoleWindow() override;

private:
	// ICastable overrides

	void* asImpl(uint64_t) const override;

private:
	// Member variables

}; // class XorgCoreFactory

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine
