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

	static RefPtr<IMonitor> GetMainMonitor();
	static vector<RefPtr<IMonitor>> GetMonitors();

public:
	// IMonitor overrides

	string GetName() const override;
	Rect GetResolution() const override;
	Rect GetWorkSpace() const override;
	uint GetRefreshFrequency() const override;

private:
	// BaseObject overrides

	virtual void * _As(uint64_t) const override;

private:
	// Member variables

	const HMONITOR m_monitor;

}; // class Win32Monitor

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine