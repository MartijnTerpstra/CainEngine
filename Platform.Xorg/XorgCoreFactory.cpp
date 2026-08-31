#include "Precomp.h"

#include "XorgCoreFactory.h"

#include "XorgWindow.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

XorgCoreFactory::XorgCoreFactory()
{
	COMMON_CALLSTACK_CALL;
}

XorgCoreFactory::~XorgCoreFactory()
{
	COMMON_CALLSTACK_CALL;
}

std::string XorgCoreFactory::getPlatformName() const
{
	COMMON_CALLSTACK_CALL;

	return "Xorg";
}

std::vector<RefPtr<IProcess>> XorgCoreFactory::getProcesses(const std::string& name)
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::getProcess(uint32_t id)
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::getCurrentProcess()
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::createNewProcess(const std::string& path)
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::createNewProcess(
	const std::string& path, const std::string& commandLine)
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::createNewProcess(
	const std::string& path, const std::string& commandLine, const std::string& workingDirectory)
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

std::vector<RefPtr<IMonitor>> XorgCoreFactory::getMonitors()
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IMonitor> XorgCoreFactory::getMainMonitor()
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IWindow> XorgCoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener)
{
	COMMON_CALLSTACK_CALL;

	return XorgWindow::createNewWindow(name, size, type, flags, listener, nullptr);
}

RefPtr<IWindow> XorgCoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener)
{
	COMMON_CALLSTACK_CALL;

	return XorgWindow::createNewWindow(name, size, type, flags, nullptr, listener);
}

RefPtr<IWindow> XorgCoreFactory::getConsoleWindow()
{
	COMMON_CALLSTACK_CALL;

	return nullptr;
}

void* XorgCoreFactory::asImpl(uint64_t typeHash) const
{
	COMMON_CALLSTACK_CALL;

	switch (typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(ICoreFactory);
		CHECK_TYPE_AND_RETURN(XorgCoreFactory);
	default:
		return nullptr;
	}
}
