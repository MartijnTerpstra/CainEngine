#include "Precomp.h"

#include "LinuxCoreFactory.h"

#include "WaylandMonitor.h"
#include "WaylandWindow.h"
#include "XorgMonitor.h"
#include "XorgWindow.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Internal;

LinuxCoreFactory::LinuxCoreFactory()
	: m_isWayland(detectWayland())
{ }

LinuxCoreFactory::~LinuxCoreFactory() = default;

std::string LinuxCoreFactory::getPlatformName() const
{
	return "Linux";
}

std::vector<RefPtr<IMonitor>> LinuxCoreFactory::getMonitors()
{
	if(m_isWayland)
		return WaylandMonitor::getMonitors();

	return XorgMonitor::getMonitors();
}

RefPtr<IMonitor> LinuxCoreFactory::getMainMonitor()
{
	if(m_isWayland)
		return WaylandMonitor::getMainMonitor();

	return XorgMonitor::getMainMonitor();
}

RefPtr<IWindow> LinuxCoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags,
	const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener)
{
	if(m_isWayland)
		return WaylandWindow::createNewWindow(name, size, type, flags, listener, nullptr);

	return XorgWindow::createNewWindow(name, size, type, flags, listener, nullptr);
}

RefPtr<IWindow> LinuxCoreFactory::createNewWindow(const std::string& name, const uint2& size,
	WindowType type, flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener)
{
	if(m_isWayland)
		return WaylandWindow::createNewWindow(name, size, type, flags, nullptr, listener);

	return XorgWindow::createNewWindow(name, size, type, flags, nullptr, listener);
}

RefPtr<IWindow> LinuxCoreFactory::getConsoleWindow()
{
	return nullptr;
}

bool LinuxCoreFactory::detectWayland()
{
	// The same probe every Wayland-aware toolkit (SDL, GLFW, ...) uses: if a compositor is
	// listening on $WAYLAND_DISPLAY (or the default "wayland-0"), prefer it over Xorg/XWayland.
	// This only probes the connection - WaylandMonitor/WaylandWindow each open their own, same
	// as XorgMonitor/XorgWindow do for their X displays.
	wl_display* display = wl_display_connect(nullptr);
	if(display == nullptr)
		return false;

	wl_display_disconnect(display);
	return true;
}

void* LinuxCoreFactory::asImpl(uint64_t typeHash) const
{
	switch(typeHash)
	{
		CHECK_TYPE_AND_RETURN(Common::BaseObject);
		CHECK_TYPE_AND_RETURN(ICoreFactory);
		CHECK_TYPE_AND_RETURN(LinuxCoreFactory);
	default:
		return nullptr;
	}
}
