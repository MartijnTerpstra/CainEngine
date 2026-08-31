#include "Precomp.h"

#include "Win32Process.h"

#include <Psapi.h>

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

Win32Process::Win32Process(HANDLE process)
	: m_process(process)
{
	COMMON_CALLSTACK_CALL;
}

Win32Process::~Win32Process()
{
	COMMON_CALLSTACK_CALL;

	if(m_process)
	{
		CloseHandle(m_process);
	}
}

std::vector<RefPtr<IProcess>> Win32Process::getProcesses(const std::string& name)
{
	COMMON_CALLSTACK_CALL;

	std::vector<RefPtr<IProcess>> retval;

	DWORD processes[1024];
	DWORD needed;

	if(!EnumProcesses(processes, sizeof(processes), &needed))
	{
		Common::fatalError("IPlatformFactory::GetProcesses(): failure getting processes");
	}

	DWORD count = needed / sizeof(DWORD);

	for(DWORD i = 0; i < count; ++i)
	{
		TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

		HANDLE processModule = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i]);

		HMODULE hMod;
		DWORD cbNeeded;

		if(EnumProcessModules(processModule, &hMod, sizeof(hMod), &cbNeeded))
		{
			GetModuleBaseNameA(
				processModule, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
		}

		if(szProcessName == name)
		{
			retval.push_back(RefPtr<Win32Process>::create(processModule));
		}
		else
		{
			CloseHandle(processModule);
		}
	}

	return retval;
}

RefPtr<IProcess> Win32Process::getProcess(uint32_t id)
{
	COMMON_CALLSTACK_CALL;

	auto handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)id);

	if(handle)
	{
		return RefPtr<Win32Process>::create(handle);
	}

	return nullptr;
}

RefPtr<IProcess> Win32Process::getCurrentProcess()
{
	COMMON_CALLSTACK_CALL;

	auto process = ::GetCurrentProcess();

	if(process != nullptr)
	{
		return RefPtr<Win32Process>::create(process);
	}

	return nullptr;
}

RefPtr<IProcess> Win32Process::createNewProcess(
	const std::string& path, const std::string& commandLine, const std::string& workingDirectory)
{
	COMMON_CALLSTACK_CALL;

	std::string cmdLine;
	if(path.find(' ') != std::string::npos)
	{
		cmdLine = "\"" + path + "\"" + commandLine;
	}
	else
	{
		cmdLine = path + commandLine;
	}

	std::string workingDir;
	if(workingDirectory.empty())
	{
		size_t lastbackslash = path.find_last_of('\\');
		size_t lastforwardslash = path.find_last_of('/');

		if(lastbackslash != std::string::npos && lastforwardslash != std::string::npos)
		{
			size_t foundPos = std::max(lastbackslash, lastforwardslash);

			workingDir = path.substr(0, foundPos);
		}
		else if(lastbackslash != std::string::npos)
		{
			workingDir = path.substr(0, lastbackslash);
		}
		else if(lastforwardslash != std::string::npos)
		{
			workingDir = path.substr(0, lastforwardslash);
		}
	}
	else
	{
		workingDir = workingDirectory;
	}

	PROCESS_INFORMATION processInfo;

	STARTUPINFOA startupInfo;

	mst::zeromem(startupInfo);
	startupInfo.cb = sizeof(startupInfo);



	if(CreateProcessA(path.c_str(), &cmdLine[0], nullptr, nullptr, FALSE, 0, nullptr,
		   workingDir.c_str(), &startupInfo, &processInfo))
	{
		CloseHandle(processInfo.hThread);
		return RefPtr<Win32Process>::create(processInfo.hProcess);
	}

	return nullptr;
}

std::string Win32Process::getName() const
{
	COMMON_CALLSTACK_CALL;

	CHAR szProcessName[MAX_PATH] = "<unknown>";

	HMODULE hMod;
	DWORD cbNeeded;

	if(EnumProcessModules(m_process, &hMod, sizeof(hMod), &cbNeeded))
	{
		GetModuleBaseNameA(m_process, hMod, szProcessName, sizeof(szProcessName) / sizeof(CHAR));
	}

	return szProcessName;
}

uint32_t Win32Process::getId() const
{
	COMMON_CALLSTACK_CALL;

	return (uint32_t)GetProcessId(m_process);
}

uint32_t Win32Process::getSessionId() const
{
	COMMON_CALLSTACK_CALL;

	DWORD sessionId = (DWORD)-1;
	if(ProcessIdToSessionId(getId(), &sessionId))
	{
		return (uint32_t)sessionId;
	}

	return (uint32_t)-1;
}


static std::chrono::system_clock::time_point fT2Tp(const FILETIME& ft)
{
	COMMON_CALLSTACK_CALL;

	// number of seconds
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;

	time_t secs = ull.QuadPart / 10000000ULL - 11644473600ULL;
	std::chrono::milliseconds ms((ull.QuadPart / 10000ULL) % 1000);

	auto tp = std::chrono::system_clock::from_time_t(secs);
	tp += ms;
	return tp;
}

std::chrono::time_point<std::chrono::system_clock> Win32Process::getCreationTime() const
{
	COMMON_CALLSTACK_CALL;

	FILETIME creation, exit, kernel, user;
	if(!GetProcessTimes(m_process, &creation, &exit, &kernel, &user))
	{
		Common::fatalError("Platform::IProcess::GetCreationTime(): internal windows failure, "
						   "GetProcessTimes failed");
	}

	SYSTEMTIME systime;
	if(!FileTimeToSystemTime(&creation, &systime))
	{
		Common::fatalError("Platform::IProcess::GetCreationTime(): internal windows failure, "
						   "FileTimeToSystemTime failed");
	}

	return fT2Tp(creation);
}

void* Win32Process::asImpl(uint64_t typeHash) const
{
	COMMON_CALLSTACK_CALL;

	switch(typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(IProcess);
		CHECK_TYPE_AND_RETURN(Win32Process);
	default:
		return nullptr;
	}
}