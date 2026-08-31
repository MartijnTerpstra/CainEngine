#pragma once

namespace CainEngine {
namespace Platform {

class IMonitor : public Common::BaseObject
{
	COMMON_DECLARE_INTERFACE(IMonitor);

public:
	// Main functionality

	/**
		Summary:
			Returns the name of the monitor
	*/
	virtual std::string getName() const = 0;

	/**
		Summary:
			Returns resolution of the monitor in pixels
	*/
	virtual Rect getResolution() const = 0;

	/**
		Summary:
			Returns work space of the monitor, in pixels

		Details:
			This is the area where there is room for client applications
	*/
	virtual Rect getWorkSpace() const = 0;

	/**
		Summary:
			return the refresh rate of the monitor in Hz

		Notes:
			This is usually 60Hz, but it could be in the range of [50, 144]
	*/
	virtual uint32_t getRefreshFrequency() const = 0;

}; // class IMonitor

inline IMonitor::~IMonitor() = default;

}; // namespace Platform
}; // namespace CainEngine