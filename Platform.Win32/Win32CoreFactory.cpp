#include "Precomp.h"

#include "Win32CoreFactory.h"

#include "Win32Process.h"
#include "Win32Window.h"
#include "Win32Monitor.h"

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

string Win32CoreFactory::GetPlatformName() const
{
	COMMON_CALLSTACK_CALL;

	return "Win32";
}

vector<RefPtr<IProcess>> Win32CoreFactory::GetProcesses(const string& name)
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::GetProcesses(name);
}

RefPtr<IProcess> Win32CoreFactory::GetProcess(uint id)
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::GetProcess(id);
}

RefPtr<IProcess> Win32CoreFactory::GetCurrentProcess()
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::GetCurrentProcess();
}

RefPtr<IProcess> Win32CoreFactory::CreateNewProcess(const string & path)
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::CreateNewProcess(path, "", "");
}

RefPtr<IProcess> Win32CoreFactory::CreateNewProcess(const string & path, const string & commandLine)
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::CreateNewProcess(path, commandLine, "");
}

RefPtr<IProcess> Win32CoreFactory::CreateNewProcess(const string & path, const string & commandLine,
	const string & workingDirectory)
{
	COMMON_CALLSTACK_CALL;

	return Win32Process::CreateNewProcess(path, commandLine, workingDirectory);
}

vector<RefPtr<IMonitor>> Win32CoreFactory::GetMonitors()
{
	COMMON_CALLSTACK_CALL;

	return Win32Monitor::GetMonitors();
}

RefPtr<IMonitor> Win32CoreFactory::GetMainMonitor()
{
	COMMON_CALLSTACK_CALL;

	return Win32Monitor::GetMainMonitor();
}

RefPtr<IWindow> Win32CoreFactory::CreateNewWindow(const string & name, const uint2 & size, WindowType type, flag<WindowFlags> flags, const shared_ptr<ClientInterfaces::IWindowEventListener>& listener)
{
	COMMON_CALLSTACK_CALL;

	return Win32Window::CreateNewWindow(name, size, type, flags, listener, nullptr);
}

RefPtr<IWindow> Win32CoreFactory::CreateNewWindow(const string & name, const uint2 & size, WindowType type, flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener)
{
	COMMON_CALLSTACK_CALL;

	return Win32Window::CreateNewWindow(name, size, type, flags, nullptr, listener);
}

RefPtr<IWindow> Win32CoreFactory::GetConsoleWindow()
{
	COMMON_CALLSTACK_CALL;

	return Win32Window::GetConsole();
}

void* CainEngine::Platform::Internal::Win32CoreFactory::_As(uint64_t typeHash) const
{
	COMMON_CALLSTACK_CALL;

	switch (typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(ICoreFactory);
		CHECK_TYPE_AND_RETURN(Win32CoreFactory);
	default:
		Common::FatalError("Invalid cast");
	}
}