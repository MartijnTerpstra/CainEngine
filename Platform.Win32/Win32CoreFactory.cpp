#include "Precomp.h"

#include "Win32CoreFactory.h"

#include "Win32Process.h"
#include "Win32Window.h"
#include "Win32Monitor.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

Win32CoreFactory::Win32CoreFactory()
{
	COMMON_CALLSTACK_CALL;
}

Win32CoreFactory::~Win32CoreFactory()
{
	COMMON_CALLSTACK_CALL;
}

std::string Win32CoreFactory::getPlatformName() const
{
	COMMON_CALLSTACK_CALL;

	return "Win32";
}

std::vector<RefPtr<IProcess>> Win32CoreFactory::getProcesses(const std::string& name)
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::getProcesses(name);
}

RefPtr<IProcess> Win32CoreFactory::getProcess(uint32_t id)
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::getProcess(id);
}

RefPtr<IProcess> Win32CoreFactory::getCurrentProcess()
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::getCurrentProcess();
}

RefPtr<IProcess> Win32CoreFactory::createNewProcess(const std::string& path)
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::createNewProcess(path, "", "");
}

RefPtr<IProcess> Win32CoreFactory::createNewProcess(
	const std::string& path, const std::string& commandLine)
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::createNewProcess(path, commandLine, "");
}

RefPtr<IProcess> Win32CoreFactory::createNewProcess(
	const std::string& path, const std::string& commandLine, const std::string& workingDirectory)
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::createNewProcess(path, commandLine, workingDirectory);
}

std::vector<RefPtr<IMonitor>> Win32CoreFactory::getMonitors()
{
	COMMON_CALLSTACK_CALL;

	return Win32Monitor::getMonitors();
}

RefPtr<IMonitor> Win32CoreFactory::getMainMonitor()
{
	COMMON_CALLSTACK_CALL;

	return Win32Monitor::getMainMonitor();
}

RefPtr<IWindow> Win32CoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener)
{
	COMMON_CALLSTACK_CALL;

	return Win32Window::createNewWindow(name, size, type, flags, listener, nullptr);
}

RefPtr<IWindow> Win32CoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener)
{
	COMMON_CALLSTACK_CALL;

	return Win32Window::createNewWindow(name, size, type, flags, nullptr, listener);
}

RefPtr<IWindow> Win32CoreFactory::getConsoleWindow()
{
	COMMON_CALLSTACK_CALL;

	return Win32Window::getConsole();
}

void* CainEngine::Platform::Internal::Win32CoreFactory::asImpl(uint64_t typeHash) const
{
	COMMON_CALLSTACK_CALL;

	switch(typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(ICoreFactory);
		CHECK_TYPE_AND_RETURN(Win32CoreFactory);
	default:
		return nullptr;
	}
}