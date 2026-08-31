#include "Precomp.h"

#include "XorgCoreFactory.h"

#include "XorgWindow.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

XorgCoreFactory::XorgCoreFactory() = default;

XorgCoreFactory::~XorgCoreFactory() = default;

std::string XorgCoreFactory::getPlatformName() const
{
	return "Xorg";
}

std::vector<RefPtr<IProcess>> XorgCoreFactory::getProcesses(const std::string& name)
{
	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::getProcess(uint32_t id)
{
	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::getCurrentProcess()
{
	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::createNewProcess(const std::string& path)
{
	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::createNewProcess(
	const std::string& path, const std::string& commandLine)
{
	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::createNewProcess(
	const std::string& path, const std::string& commandLine, const std::string& workingDirectory)
{
	return {}; // TODO: not implemented
}

std::vector<RefPtr<IMonitor>> XorgCoreFactory::getMonitors()
{
	return {}; // TODO: not implemented
}

RefPtr<IMonitor> XorgCoreFactory::getMainMonitor()
{
	return {}; // TODO: not implemented
}

RefPtr<IWindow> XorgCoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener)
{
	return XorgWindow::createNewWindow(name, size, type, flags, listener, nullptr);
}

RefPtr<IWindow> XorgCoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener)
{
	return XorgWindow::createNewWindow(name, size, type, flags, nullptr, listener);
}

RefPtr<IWindow> XorgCoreFactory::getConsoleWindow()
{
	return nullptr;
}

void* XorgCoreFactory::asImpl(uint64_t typeHash) const
{
	switch(typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(ICoreFactory);
		CHECK_TYPE_AND_RETURN(XorgCoreFactory);
	default:
		return nullptr;
	}
}
