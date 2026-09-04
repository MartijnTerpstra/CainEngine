#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

/*
	Summary:
		Linux implementation of the IPlatformFactory
*/
class LinuxCoreFactory final : public ICoreFactory
{
	COMMON_DECLARE_NON_COPY(LinuxCoreFactory);
	friend class Common::RefPtr<LinuxCoreFactory>;

private:
	// ctor & dtor

	LinuxCoreFactory();
	~LinuxCoreFactory();

public:
	// IPlatformFactory overrides

	std::string getPlatformName() const override;

	std::vector<RefPtr<IMonitor>> getMonitors() override;

	RefPtr<IMonitor> getMainMonitor() override;

	RefPtr<IWindow> createNewWindow(const std::string& name, const uint2& size, WindowType type,
		flag<WindowFlags> flags,
		const std::shared_ptr<ClientInterfaces::IWindowEventListener>& listener) override;

	RefPtr<IWindow> createNewWindow(const std::string& name, const uint2& size, WindowType type,
		flag<WindowFlags> flags, ClientInterfaces::IWindowEventListener* listener) override;

	RefPtr<IWindow> getConsoleWindow() override;

private:
	// ICastable overrides

	void* asImpl(uint64_t) const override;

private:
	// Member variables

}; // class LinuxCoreFactory

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine
