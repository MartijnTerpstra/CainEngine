#include "Precomp.h"

#include "XorgMonitor.h"

#include <X11/extensions/Xrandr.h>

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

XorgMonitor::XorgMonitor(std::string name, Rect resolution, uint32_t refreshFrequency)
	: m_name(std::move(name))
	, m_resolution(resolution)
	, m_refreshFrequency(refreshFrequency)
{ }

XorgMonitor::~XorgMonitor() = default;

static uint32_t getModeRefreshFrequency(const XRRModeInfo& mode)
{
	const uint64_t denominator = uint64_t(mode.hTotal) * uint64_t(mode.vTotal);
	if(denominator == 0)
		return 0;

	// dotClock is in Hz, round to the nearest whole refresh rate
	return static_cast<uint32_t>((uint64_t(mode.dotClock) + denominator / 2) / denominator);
}

std::vector<RefPtr<IMonitor>> XorgMonitor::getMonitors()
{
	Display* display = XOpenDisplay(":0.0");
	if(display == nullptr)
	{
		Common::fatalError("XorgMonitor::getMonitors(): XOpenDisplay failed");
	}

	::Window root = XDefaultRootWindow(display);

	XRRScreenResources* resources = XRRGetScreenResourcesCurrent(display, root);
	if(resources == nullptr)
	{
		XCloseDisplay(display);
		Common::fatalError("XorgMonitor::getMonitors(): call failed: XRRGetScreenResourcesCurrent");
	}

	RROutput primaryOutput = XRRGetOutputPrimary(display, root);

	std::vector<RefPtr<IMonitor>> monitors;
	monitors.reserve(resources->noutput);

	for(int i = 0; i < resources->noutput; ++i)
	{
		XRROutputInfo* outputInfo = XRRGetOutputInfo(display, resources, resources->outputs[i]);
		if(outputInfo == nullptr)
			continue;

		if(outputInfo->connection != RR_Connected || outputInfo->crtc == 0)
		{
			XRRFreeOutputInfo(outputInfo);
			continue;
		}

		XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(display, resources, outputInfo->crtc);
		if(crtcInfo == nullptr)
		{
			XRRFreeOutputInfo(outputInfo);
			continue;
		}

		uint32_t refreshFrequency = 0;
		for(int m = 0; m < resources->nmode; ++m)
		{
			if(resources->modes[m].id == crtcInfo->mode)
			{
				refreshFrequency = getModeRefreshFrequency(resources->modes[m]);
				break;
			}
		}

		Rect resolution(crtcInfo->x, crtcInfo->y, crtcInfo->x + int(crtcInfo->width),
			crtcInfo->y + int(crtcInfo->height));

		auto monitor = RefPtr<XorgMonitor>::create(
			std::string(outputInfo->name, outputInfo->nameLen), resolution, refreshFrequency);

		if(resources->outputs[i] == primaryOutput)
		{
			monitors.insert(monitors.begin(), std::move(monitor));
		}
		else
		{
			monitors.push_back(std::move(monitor));
		}

		XRRFreeCrtcInfo(crtcInfo);
		XRRFreeOutputInfo(outputInfo);
	}

	XRRFreeScreenResources(resources);
	XCloseDisplay(display);

	return monitors;
}

RefPtr<IMonitor> XorgMonitor::getMainMonitor()
{
	auto monitors = getMonitors();
	if(monitors.empty())
		return nullptr;

	return monitors.front();
}

std::string XorgMonitor::getName() const
{
	return m_name;
}

Rect XorgMonitor::getResolution() const
{
	return m_resolution;
}

Rect XorgMonitor::getWorkSpace() const
{
	// X11 has no window-manager-independent notion of a per-monitor work area (panel/taskbar
	// reservations are tracked by the window manager, e.g. via _NET_WORKAREA, and are not
	// resolved per output); approximate it with the full monitor resolution.
	return m_resolution;
}

uint32_t XorgMonitor::getRefreshFrequency() const
{
	return m_refreshFrequency;
}

void* XorgMonitor::asImpl(uint64_t typeHash) const
{
	switch(typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(IMonitor);
		CHECK_TYPE_AND_RETURN(XorgMonitor);
	default:
		return nullptr;
	}
}
