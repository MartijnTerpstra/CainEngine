#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

class XorgMonitor final : public IMonitor
{
	friend class RefPtr<XorgMonitor>;

private:
	// ctor & dtor

	XorgMonitor(std::string name, Rect resolution, uint32_t refreshFrequency);
	~XorgMonitor();

	COMMON_DECLARE_NON_COPY(XorgMonitor);

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

	const std::string m_name;
	const Rect m_resolution;
	const uint32_t m_refreshFrequency;

}; // class XorgMonitor

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine
