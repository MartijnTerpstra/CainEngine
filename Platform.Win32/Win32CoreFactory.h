#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

/*
	Summary:
		Windows implementation of the IPlatformFactory
*/
class Win32CoreFactory final : public ICoreFactory
{
	COMMON_DECLARE_NON_COPY(Win32CoreFactory);
	friend class Common::RefPtr<Win32CoreFactory>;

private:
	// ctor & dtor

	Win32CoreFactory();
	~Win32CoreFactory();

public:
	// IPlatformFactory overrides

	std::string getPlatformName() const override;

	std::vector<RefPtr<IProcess>> getProcesses(const std::string& name) override;

	RefPtr<IProcess> getProcess(uint32_t id) override;

	RefPtr<IProcess> getCurrentProcess() override;

	RefPtr<IProcess> createNewProcess(const std::string& path) override;

	RefPtr<IProcess> createNewProcess(
		const std::string& path, const std::string& commandLine) override;

	RefPtr<IProcess> createNewProcess(const std::string& path, const std::string& commandLine,
		const std::string& workingDirectory) override;

	std::vector<RefPtr<IMonitor>> getMonitors() override;

	RefPtr<IMonitor> getMainMonitor() override;

	RefPtr<IWindow> createNewWindow(const std::string& name, const uint2& size, WindowType type,
		flag<WindowFlags> flags,
		const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener) override;

	RefPtr<IWindow> createNewWindow(const std::string& name, const uint2& size, WindowType type,
		flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener) override;

	RefPtr<IWindow> getConsoleWindow() override;

private:
	// ICastable overrides

	void* asImpl(uint64_t) const override;

private:
	// Member variables

}; // class Win32CoreFactory

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine