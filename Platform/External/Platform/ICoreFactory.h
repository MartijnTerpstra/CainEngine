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

	virtual std::string GetPlatformName() const = 0;

	virtual std::vector<RefPtr<IProcess>> GetProcesses(const std::string& name) = 0;

	virtual RefPtr<IProcess> GetProcess(uint id) = 0;

	virtual RefPtr<IProcess> GetCurrentProcess() = 0;

	virtual RefPtr<IProcess> CreateNewProcess(const std::string& path) = 0;

	virtual RefPtr<IProcess> CreateNewProcess(const std::string& path, const std::string& commandLine) = 0;

	virtual RefPtr<IProcess> CreateNewProcess(const std::string& path, const std::string& commandLine, const std::string& workingDirectory) = 0;

	virtual std::vector<RefPtr<IMonitor>> GetMonitors() = 0;

	virtual RefPtr<IMonitor> GetMainMonitor() = 0;

	virtual RefPtr<IWindow> CreateNewWindow(const std::string& name, const uint2& size, WindowType type, flag<WindowFlags> flags, const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener) = 0;

	virtual RefPtr<IWindow> CreateNewWindow(const std::string& name, const uint2& size, WindowType type, flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener) = 0;

	virtual RefPtr<IWindow> GetConsoleWindow() = 0;

}; // class ICoreFactory

inline ICoreFactory::~ICoreFactory() = default;

}; // namespace Platform
}; // namespace CainEngine