#include "Precomp.h"

#include "LinuxCoreFactory.h"

#include "LinuxWindow.h"

using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

LinuxCoreFactory::LinuxCoreFactory()
{
	COMMON_CALLSTACK_CALL;
}

LinuxCoreFactory::~LinuxCoreFactory()
{
	COMMON_CALLSTACK_CALL;
}

string LinuxCoreFactory::GetPlatformName() const
{
	COMMON_CALLSTACK_CALL;

	return "Win32";
}

vector<shared_ptr<IProcess>> LinuxCoreFactory::GetProcesses(const string& name)
{
	COMMON_CALLSTACK_CALL;

	return {};//return Win32Process::GetProcesses(name);
}

shared_ptr<IProcess> LinuxCoreFactory::GetProcess(uint id)
{
	COMMON_CALLSTACK_CALL;

	return {};//return Win32Process::GetProcess(id);
}

shared_ptr<IProcess> LinuxCoreFactory::GetCurrentProcess()
{
	COMMON_CALLSTACK_CALL;

	return {};//return Win32Process::GetCurrentProcess();
}

shared_ptr<IProcess> LinuxCoreFactory::CreateNewProcess(const string & path)
{
	COMMON_CALLSTACK_CALL;

	return {};//return Win32Process::CreateNewProcess(path, "", "");
}

shared_ptr<IProcess> LinuxCoreFactory::CreateNewProcess(const string & path, const string & commandLine)
{
	COMMON_CALLSTACK_CALL;

	return {};//return Win32Process::CreateNewProcess(path, commandLine, "");
}

shared_ptr<IProcess> LinuxCoreFactory::CreateNewProcess(const string & path, const string & commandLine,
	const string & workingDirectory)
{
	COMMON_CALLSTACK_CALL;

	return {};//return Win32Process::CreateNewProcess(path, commandLine, workingDirectory);
}

vector<shared_ptr<IMonitor>> LinuxCoreFactory::GetMonitors()
{
	COMMON_CALLSTACK_CALL;

	return {};//return Win32Monitor::GetMonitors();
}

shared_ptr<IMonitor> LinuxCoreFactory::GetMainMonitor()
{
	COMMON_CALLSTACK_CALL;

	return {};//return Win32Monitor::GetMainMonitor();
}

shared_ptr<IWindow> LinuxCoreFactory::CreateNewWindow(const string & name, const uint2 & size, WindowType type, flag<WindowFlags> flags, const weak_ptr<ClientInterfaces::IWindowEventListener>& listener)
{
	COMMON_CALLSTACK_CALL;

	return LinuxWindow::CreateNewWindow(name, size, type, flags, listener);
}

shared_ptr<IWindow> LinuxCoreFactory::GetConsoleWindow()
{
	COMMON_CALLSTACK_CALL;

	return null;
}