#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

class Win32Monitor final : public IMonitor
{
	friend class RefPtr<Win32Monitor>;

private:
	// ctor & dtor

	Win32Monitor(HMONITOR monitor);
	~Win32Monitor();

	COMMON_DECLARE_NON_COPY(Win32Monitor);

public:
	// Creation

	static RefPtr<IMonitor> getMainMonitor();
	static std::vector<RefPtr<IMonitor>> getMonitors();

public:
	// IMonitor overrides

	std::string getName() const override;
	Rect getResolution() const override;
	Rect getWorkSpace() const override;
	uint32_t getRefreshFrequency() const override;

private:
	// BaseObject overrides

	void* asImpl(uint64_t) const override;

private:
	// Member variables

	const HMONITOR m_monitor;

}; // class Win32Monitor

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine