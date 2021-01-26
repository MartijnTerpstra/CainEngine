#pragma once

namespace CainEngine {
namespace Platform {

/**
	Summary:
		A factory to create platform-specific objects
*/
class ICoreFactory : public Common::BaseObject
{
	COMMON_DECLARE_INTERFACE(ICoreFactory);

public:
	// Main functionality

	virtual string GetPlatformName() const = 0;

	virtual vector<RefPtr<IProcess>> GetProcesses(const string& name) = 0;

	virtual RefPtr<IProcess> GetProcess(uint id) = 0;

	virtual RefPtr<IProcess> GetCurrentProcess() = 0;

	virtual RefPtr<IProcess> CreateNewProcess(const string& path) = 0;

	virtual RefPtr<IProcess> CreateNewProcess(const string& path, const string& commandLine) = 0;

	virtual RefPtr<IProcess> CreateNewProcess(const string& path, const string& commandLine, const string& workingDirectory) = 0;

	virtual vector<RefPtr<IMonitor>> GetMonitors() = 0;

	virtual RefPtr<IMonitor> GetMainMonitor() = 0;

	virtual RefPtr<IWindow> CreateNewWindow(const string& name, const uint2& size, WindowType type, flag<WindowFlags> flags, const shared_ptr<ClientInterfaces::IWindowEventListener>& listener) = 0;

	virtual RefPtr<IWindow> CreateNewWindow(const string& name, const uint2& size, WindowType type, flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener) = 0;

	virtual RefPtr<IWindow> GetConsoleWindow() = 0;

}; // class ICoreFactory

inline ICoreFactory::~ICoreFactory() = default;

}; // namespace Platform
}; // namespace CainEngine