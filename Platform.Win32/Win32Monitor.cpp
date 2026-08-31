#include "Precomp.h"

#include "Win32Monitor.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

Win32Monitor::Win32Monitor(HMONITOR monitor)
	: m_monitor(monitor)
{
	COMMON_CALLSTACK_CALL;
}

Win32Monitor::~Win32Monitor()
{
	COMMON_CALLSTACK_CALL;
}

static BOOL __stdcall getMainMonitorProc(HMONITOR monitor, HDC, LPRECT, LPARAM param)
{
	COMMON_CALLSTACK_CALL;

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if(!GetMonitorInfoA(monitor, &info))
	{
		Common::fatalError("Win32Monitor::GetResolution(): call failed: GetMonitorInfoA");
	}

	if(info.dwFlags & MONITORINFOF_PRIMARY)
	{
		*reinterpret_cast<HMONITOR*>(param) = monitor;
		return FALSE;
	}

	return TRUE;
}

RefPtr<IMonitor> Win32Monitor::getMainMonitor()
{
	COMMON_CALLSTACK_CALL;

	HMONITOR monitor = 0;
	if(!EnumDisplayMonitors(
		   nullptr, nullptr, getMainMonitorProc, reinterpret_cast<LPARAM>(&monitor)))
	{
		Common::fatalError("Win32Monitor::GetMainMonitor(): call failed: EnumDisplayMonitors");
	}

	if(monitor)
	{
		return RefPtr<Win32Monitor>::create(monitor);
	}

	return nullptr;
}

static BOOL __stdcall getMonitorsProc(HMONITOR monitor, HDC, LPRECT, LPARAM param)
{
	COMMON_CALLSTACK_CALL;

	auto monitors = reinterpret_cast<std::vector<HMONITOR>*>(param);

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if(!GetMonitorInfoA(monitor, &info))
	{
		Common::fatalError("Win32Monitor::GetResolution(): call failed: GetMonitorInfoA");
	}

	if(info.dwFlags & MONITORINFOF_PRIMARY)
	{
		monitors->insert(monitors->begin(), monitor);
	}
	else
	{
		monitors->push_back(monitor);
	}

	return TRUE;
}

std::vector<RefPtr<IMonitor>> Win32Monitor::getMonitors()
{
	COMMON_CALLSTACK_CALL;

	std::vector<HMONITOR> monitors;

	if(!EnumDisplayMonitors(nullptr, nullptr, getMonitorsProc, reinterpret_cast<LPARAM>(&monitors)))
	{
		Common::fatalError("Win32Monitor::GetMainMonitor(): call failed: EnumDisplayMonitors");
	}

	if(monitors.empty())
		return std::vector<RefPtr<IMonitor>>();

	std::vector<RefPtr<IMonitor>> retvals;
	retvals.reserve(monitors.size());

	for(auto monitor : monitors)
	{
		retvals.push_back(RefPtr<Win32Monitor>::create(monitor));
	}

	return retvals;
}

std::string Win32Monitor::getName() const
{
	COMMON_CALLSTACK_CALL;

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if(!GetMonitorInfoA(m_monitor, &info))
	{
		Common::fatalError("Win32Monitor::GetRefreshFrequency(): call failed: GetMonitorInfoA");
	}

	DEVMODEA devMode;

	if(!EnumDisplaySettingsA(info.szDevice, ENUM_CURRENT_SETTINGS, &devMode))
	{
		Common::fatalError(
			"Win32Monitor::GetRefreshFrequency(): call failed: EnumDisplaySettingsA");
	}

	return (const char*)devMode.dmFormName;
}

Rect Win32Monitor::getResolution() const
{
	COMMON_CALLSTACK_CALL;

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if(!GetMonitorInfoA(m_monitor, &info))
	{
		Common::fatalError("Win32Monitor::GetResolution(): call failed: GetMonitorInfoA");
	}

	return Rect(
		info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right, info.rcMonitor.bottom);
}

Rect Win32Monitor::getWorkSpace() const
{
	COMMON_CALLSTACK_CALL;

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if(!GetMonitorInfoA(m_monitor, &info))
	{
		Common::fatalError("Win32Monitor::GetWorkSpace(): call failed: GetMonitorInfoA");
	}

	return Rect(info.rcWork.left, info.rcWork.top, info.rcWork.right, info.rcWork.bottom);
}

uint32_t Win32Monitor::getRefreshFrequency() const
{
	COMMON_CALLSTACK_CALL;

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if(!GetMonitorInfoA(m_monitor, &info))
	{
		Common::fatalError("Win32Monitor::GetRefreshFrequency(): call failed: GetMonitorInfoA");
	}

	DEVMODEA devMode;

	if(!EnumDisplaySettingsA(info.szDevice, ENUM_CURRENT_SETTINGS, &devMode))
	{
		Common::fatalError(
			"Win32Monitor::GetRefreshFrequency(): call failed: EnumDisplaySettingsA");
	}

	return devMode.dmDisplayFrequency;
}

void* Win32Monitor::asImpl(uint64_t typeHash) const
{
	COMMON_CALLSTACK_CALL;

	switch(typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(IMonitor);
		CHECK_TYPE_AND_RETURN(Win32Monitor);
	default:
		return nullptr;
	}
}