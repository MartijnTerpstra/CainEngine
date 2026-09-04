#include "Precomp.h"

#include "LinuxCoreFactory.h"

#include "XorgWindow.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

LinuxCoreFactory::LinuxCoreFactory() = default;

LinuxCoreFactory::~LinuxCoreFactory() = default;

std::string LinuxCoreFactory::getPlatformName() const
{
	return "Linux";
}

std::vector<RefPtr<IProcess>> LinuxCoreFactory::getProcesses(const std::string& name)
{
	return {}; // TODO: not implemented
}

RefPtr<IProcess> LinuxCoreFactory::getProcess(uint32_t id)
{
	return {}; // TODO: not implemented
}

RefPtr<IProcess> LinuxCoreFactory::getCurrentProcess()
{
	return {}; // TODO: not implemented
}

RefPtr<IProcess> LinuxCoreFactory::createNewProcess(const std::string& path)
{
	return {}; // TODO: not implemented
}

RefPtr<IProcess> LinuxCoreFactory::createNewProcess(
	const std::string& path, const std::string& commandLine)
{
	return {}; // TODO: not implemented
}

RefPtr<IProcess> LinuxCoreFactory::createNewProcess(
	const std::string& path, const std::string& commandLine, const std::string& workingDirectory)
{
	return {}; // TODO: not implemented
}

std::vector<RefPtr<IMonitor>> LinuxCoreFactory::getMonitors()
{
	return {}; // TODO: not implemented
}

RefPtr<IMonitor> LinuxCoreFactory::getMainMonitor()
{
	return {}; // TODO: not implemented
}

RefPtr<IWindow> LinuxCoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener)
{
	return XorgWindow::createNewWindow(name, size, type, flags, listener, nullptr);
}

RefPtr<IWindow> LinuxCoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener)
{
	return XorgWindow::createNewWindow(name, size, type, flags, nullptr, listener);
}

RefPtr<IWindow> LinuxCoreFactory::getConsoleWindow()
{
	return nullptr;
}

void* LinuxCoreFactory::asImpl(uint64_t typeHash) const
{
	switch(typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(ICoreFactory);
		CHECK_TYPE_AND_RETURN(LinuxCoreFactory);
	default:
		return nullptr;
	}
}
