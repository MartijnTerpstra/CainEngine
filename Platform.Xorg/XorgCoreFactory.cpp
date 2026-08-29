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

string XorgCoreFactory::GetPlatformName() const
{
	COMMON_CALLSTACK_CALL;

	return "Xorg";
}

vector<RefPtr<IProcess>> XorgCoreFactory::GetProcesses(const string& name)
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::GetProcess(uint32_t id)
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::GetCurrentProcess()
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::CreateNewProcess(const string& path)
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::CreateNewProcess(
	const string& path, const string& commandLine)
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IProcess> XorgCoreFactory::CreateNewProcess(
	const string& path, const string& commandLine, const string& workingDirectory)
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

vector<RefPtr<IMonitor>> XorgCoreFactory::GetMonitors()
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IMonitor> XorgCoreFactory::GetMainMonitor()
{
	COMMON_CALLSTACK_CALL;

	return {}; // TODO: not implemented
}

RefPtr<IWindow> XorgCoreFactory::CreateNewWindow(const string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener)
{
	COMMON_CALLSTACK_CALL;

	return XorgWindow::CreateNewWindow(name, size, type, flags, listener, nullptr);
}

RefPtr<IWindow> XorgCoreFactory::CreateNewWindow(const string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener)
{
	COMMON_CALLSTACK_CALL;

	return XorgWindow::CreateNewWindow(name, size, type, flags, nullptr, listener);
}

RefPtr<IWindow> XorgCoreFactory::GetConsoleWindow()
{
	COMMON_CALLSTACK_CALL;

	return null;
}

void* XorgCoreFactory::_As(uint64_t typeHash) const
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
