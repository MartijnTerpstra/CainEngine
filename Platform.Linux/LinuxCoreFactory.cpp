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

std::vector<RefPtr<IMonitor>> XorgCoreFactory::getMonitors()
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
