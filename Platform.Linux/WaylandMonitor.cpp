#include "Precomp.h"

#include "WaylandMonitor.h"

#include <cstring>

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

namespace
{

struct OutputInfo
{
	std::string make;
	std::string model;
	int32_t x = 0;
	int32_t y = 0;
	int32_t width = 0;
	int32_t height = 0;
	int32_t refreshMilliHz = 0;
};

void handleOutputGeometry(void* data, wl_output*, int32_t x, int32_t y, int32_t /*physicalWidth*/,
	int32_t /*physicalHeight*/, int32_t /*subpixel*/, const char* make, const char* model,
	int32_t /*transform*/)
{
	auto& info = *static_cast<OutputInfo*>(data);
	info.x = x;
	info.y = y;
	info.make = make;
	info.model = model;
}

void handleOutputMode(
	void* data, wl_output*, uint32_t flags, int32_t width, int32_t height, int32_t refresh)
{
	if((flags & WL_OUTPUT_MODE_CURRENT) == 0)
		return;

	auto& info = *static_cast<OutputInfo*>(data);
	info.width = width;
	info.height = height;
	info.refreshMilliHz = refresh;
}

// done/scale/name/description are all version-2+ (name/description version-4+) events; every
// output below is bound at version 1, so the compositor never sends them and leaving those
// callbacks unset (null) here is safe.
const wl_output_listener g_outputListener = {
	.geometry = &handleOutputGeometry,
	.mode = &handleOutputMode,
};

struct RegistryState
{
	std::vector<wl_output*> outputProxies;
	std::vector<std::unique_ptr<OutputInfo>> outputs;
};

void handleRegistryGlobal(
	void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t /*version*/)
{
	if(std::strcmp(interface, wl_output_interface.name) != 0)
		return;

	auto& state = *static_cast<RegistryState*>(data);

	// Bind at version 1: geometry/mode are all this needs, and every wl_output supports
	// version 1 regardless of what the compositor advertises.
	auto* output =
		static_cast<wl_output*>(wl_registry_bind(registry, name, &wl_output_interface, 1));

	auto info = std::make_unique<OutputInfo>();
	wl_output_add_listener(output, &g_outputListener, info.get());

	state.outputProxies.push_back(output);
	state.outputs.push_back(std::move(info));
}

void handleRegistryGlobalRemove(void*, wl_registry*, uint32_t)
{ }

const wl_registry_listener g_registryListener = {
	.global = &handleRegistryGlobal,
	.global_remove = &handleRegistryGlobalRemove,
};

} // namespace

WaylandMonitor::WaylandMonitor(std::string name, Rect resolution, uint32_t refreshFrequency)
	: m_name(std::move(name))
	, m_resolution(resolution)
	, m_refreshFrequency(refreshFrequency)
{ }

WaylandMonitor::~WaylandMonitor() = default;

std::vector<RefPtr<IMonitor>> WaylandMonitor::getMonitors()
{
	wl_display* display = wl_display_connect(nullptr);
	if(display == nullptr)
	{
		Common::fatalError("WaylandMonitor::getMonitors(): wl_display_connect failed");
	}

	wl_registry* registry = wl_display_get_registry(display);

	RegistryState state;
	wl_registry_add_listener(registry, &g_registryListener, &state);

	// One roundtrip to receive the registry's "global" announcements (this is where every
	// wl_output gets bound and has its listener attached)...
	wl_display_roundtrip(display);

	// ...and a second to let each of those freshly-bound wl_outputs finish sending its
	// geometry/mode events, which are only queued once the binding above has completed.
	wl_display_roundtrip(display);

	std::vector<RefPtr<IMonitor>> monitors;
	monitors.reserve(state.outputs.size());

	for(size_t i = 0; i < state.outputs.size(); ++i)
	{
		const OutputInfo& info = *state.outputs[i];

		std::string name = info.make;
		if(!info.model.empty())
		{
			if(!name.empty())
				name += ' ';
			name += info.model;
		}

		Rect resolution(info.x, info.y, info.x + info.width, info.y + info.height);

		uint32_t refreshFrequency =
			info.refreshMilliHz > 0 ? uint32_t((info.refreshMilliHz + 500) / 1000) : 0;

		monitors.push_back(
			RefPtr<WaylandMonitor>::create(std::move(name), resolution, refreshFrequency));

		wl_output_destroy(state.outputProxies[i]);
	}

	wl_registry_destroy(registry);
	wl_display_disconnect(display);

	return monitors;
}

RefPtr<IMonitor> WaylandMonitor::getMainMonitor()
{
	auto monitors = getMonitors();
	if(monitors.empty())
		return nullptr;

	// Unlike XRandR, the core Wayland protocol has no notion of a "primary" output; fall back
	// to whichever one the compositor announced first (mirrors the approximation in
	// getWorkSpace() below).
	return monitors.front();
}

std::string WaylandMonitor::getName() const
{
	return m_name;
}

Rect WaylandMonitor::getResolution() const
{
	return m_resolution;
}

Rect WaylandMonitor::getWorkSpace() const
{
	// Same approximation as XorgMonitor::getWorkSpace(): Wayland has no compositor-independent
	// per-output usable-area concept either, so use the full monitor resolution.
	return m_resolution;
}

uint32_t WaylandMonitor::getRefreshFrequency() const
{
	return m_refreshFrequency;
}

void* WaylandMonitor::asImpl(uint64_t typeHash) const
{
	switch(typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(IMonitor);
		CHECK_TYPE_AND_RETURN(WaylandMonitor);
	default:
		return nullptr;
	}
}
