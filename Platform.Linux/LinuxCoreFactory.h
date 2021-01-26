#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

/*
	Summary:
		Linux implementation of the IPlatformFactory
*/
class LinuxCoreFactory final : public Linux::ILinuxCoreFactory, public Common::Castable<LinuxCoreFactory>
{
public:
	// ctor & dtor

	LinuxCoreFactory();
	~LinuxCoreFactory();

	COMMON_DECLARE_NON_COPY(LinuxCoreFactory);

public:
	// IPlatformFactory overrides

	string GetPlatformName() const override;

	vector<shared_ptr<IProcess>> GetProcesses(const string& name) override;

	shared_ptr<IProcess> GetProcess(uint id) override;

	shared_ptr<IProcess> GetCurrentProcess() override;

	shared_ptr<IProcess> CreateNewProcess(const string& path) override;

	shared_ptr<IProcess> CreateNewProcess(const string& path, const string& commandLine) override;

	shared_ptr<IProcess> CreateNewProcess(const string& path, const string& commandLine, const string& workingDirectory) override;

	vector<shared_ptr<IMonitor>> GetMonitors() override;

	shared_ptr<IMonitor> GetMainMonitor() override;

	shared_ptr<IWindow> CreateNewWindow(const string& name, const uint2& size, WindowType type, flag<WindowFlags> flags, const weak_ptr<ClientInterfaces::IWindowEventListener>& listener) override;

	shared_ptr<IWindow> GetConsoleWindow() override;

private:
	// Member variables

}; // class ILinuxCoreFactory

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine