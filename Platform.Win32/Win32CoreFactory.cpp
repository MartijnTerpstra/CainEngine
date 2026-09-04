#include "Precomp.h"

#include "Win32CoreFactory.h"

#include "Win32Window.h"
#include "Win32Monitor.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

Win32CoreFactory::Win32CoreFactory() = default;

Win32CoreFactory::~Win32CoreFactory() = default;

std::string Win32CoreFactory::getPlatformName() const
{
	return "Win32";
}

std::vector<RefPtr<IMonitor>> Win32CoreFactory::getMonitors()
{
	return Win32Monitor::getMonitors();
}

RefPtr<IMonitor> Win32CoreFactory::getMainMonitor()
{
	return Win32Monitor::getMainMonitor();
}

RefPtr<IWindow> Win32CoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener)
{
	return Win32Window::createNewWindow(name, size, type, flags, listener, nullptr);
}

RefPtr<IWindow> Win32CoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener)
{
	return Win32Window::createNewWindow(name, size, type, flags, nullptr, listener);
}

RefPtr<IWindow> Win32CoreFactory::getConsoleWindow()
{
	return Win32Window::getConsole();
}

void* CainEngine::Platform::Internal::Win32CoreFactory::asImpl(uint64_t typeHash) const
{
	switch(typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(ICoreFactory);
		CHECK_TYPE_AND_RETURN(Win32CoreFactory);
	default:
		return nullptr;
	}
}