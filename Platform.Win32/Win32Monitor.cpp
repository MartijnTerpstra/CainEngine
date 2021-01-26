#include "Precomp.h"

#include "Win32Monitor.h"

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

static BOOL __stdcall GetMainMonitorProc(HMONITOR monitor, HDC, LPRECT, LPARAM param)
{
	COMMON_CALLSTACK_CALL;

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if (!GetMonitorInfoA(monitor, &info))
	{
		Common::FatalError("Win32Monitor::GetResolution(): call failed: GetMonitorInfoA");
	}

	if (info.dwFlags & MONITORINFOF_PRIMARY)
	{
		*reinterpret_cast<HMONITOR*>(param) = monitor;
		return FALSE;
	}

	return TRUE;
}

RefPtr<IMonitor> Win32Monitor::GetMainMonitor()
{
	COMMON_CALLSTACK_CALL;

	HMONITOR monitor = 0;
	if (!EnumDisplayMonitors(nullptr, nullptr, GetMainMonitorProc, reinterpret_cast<LPARAM>(&monitor)))
	{
		Common::FatalError("Win32Monitor::GetMainMonitor(): call failed: EnumDisplayMonitors");
	}

	if (monitor)
	{
		return RefPtr<Win32Monitor>::Create(monitor);
	}

	return nullptr;
}

static BOOL __stdcall GetMonitorsProc(HMONITOR monitor, HDC, LPRECT, LPARAM param)
{
	COMMON_CALLSTACK_CALL;

	auto monitors = reinterpret_cast<vector<HMONITOR>*>(param);

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if (!GetMonitorInfoA(monitor, &info))
	{
		Common::FatalError("Win32Monitor::GetResolution(): call failed: GetMonitorInfoA");
	}

	if (info.dwFlags & MONITORINFOF_PRIMARY)
	{
		monitors->insert(monitors->begin(),monitor);
	}
	else
	{
		monitors->push_back(monitor);
	}

	return TRUE;
}

vector<RefPtr<IMonitor>> Win32Monitor::GetMonitors()
{
	COMMON_CALLSTACK_CALL;

	vector<HMONITOR> monitors;

	if (!EnumDisplayMonitors(nullptr, nullptr, GetMonitorsProc, reinterpret_cast<LPARAM>(&monitors)))
	{
		Common::FatalError("Win32Monitor::GetMainMonitor(): call failed: EnumDisplayMonitors");
	}

	if (monitors.empty())
		return vector<RefPtr<IMonitor>>();

	vector<RefPtr<IMonitor>> retvals;
	retvals.reserve(monitors.size());

	for (auto monitor : monitors)
	{
		retvals.push_back(RefPtr<Win32Monitor>::Create(monitor));
	}

	return retvals;
}

string Win32Monitor::GetName() const
{
	COMMON_CALLSTACK_CALL;

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if (!GetMonitorInfoA(m_monitor, &info))
	{
		Common::FatalError("Win32Monitor::GetRefreshFrequency(): call failed: GetMonitorInfoA");
	}

	DEVMODEA devMode;

	if (!EnumDisplaySettingsA(info.szDevice, ENUM_CURRENT_SETTINGS, &devMode))
	{
		Common::FatalError("Win32Monitor::GetRefreshFrequency(): call failed: EnumDisplaySettingsA");
	}

	return (const char*)devMode.dmFormName;
}

Rect Win32Monitor::GetResolution() const
{
	COMMON_CALLSTACK_CALL;

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if (!GetMonitorInfoA(m_monitor, &info))
	{
		Common::FatalError("Win32Monitor::GetResolution(): call failed: GetMonitorInfoA");
	}

	return Rect(info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right, info.rcMonitor.bottom);
}

Rect Win32Monitor::GetWorkSpace() const
{
	COMMON_CALLSTACK_CALL;

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if (!GetMonitorInfoA(m_monitor, &info))
	{
		Common::FatalError("Win32Monitor::GetWorkSpace(): call failed: GetMonitorInfoA");
	}

	return Rect(info.rcWork.left, info.rcWork.top, info.rcWork.right, info.rcWork.bottom);
}

uint Win32Monitor::GetRefreshFrequency() const
{
	COMMON_CALLSTACK_CALL;

	MONITORINFOEXA info;
	info.cbSize = sizeof(info);
	if (!GetMonitorInfoA(m_monitor, &info))
	{
		Common::FatalError("Win32Monitor::GetRefreshFrequency(): call failed: GetMonitorInfoA");
	}

	DEVMODEA devMode;

	if (!EnumDisplaySettingsA(info.szDevice, ENUM_CURRENT_SETTINGS, &devMode))
	{
		Common::FatalError("Win32Monitor::GetRefreshFrequency(): call failed: EnumDisplaySettingsA");
	}

	return devMode.dmDisplayFrequency;
}

void* Win32Monitor::_As(uint64_t typeHash) const
{
	COMMON_CALLSTACK_CALL;

	switch (typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(IMonitor);
		CHECK_TYPE_AND_RETURN(Win32Monitor);
	default:
		Common::FatalError("Invalid cast");
	}
}