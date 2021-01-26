#pragma once

namespace CainEngine {
namespace Platform {

/**
	Summary:
		Interface to a process
*/
class IProcess : public Common::BaseObject
{
	COMMON_DECLARE_INTERFACE(IProcess);

public:
	// Main functionality

	/**
		Summary:
			Returns the process's name
	*/
	virtual string GetName() const = 0;

	/**
		Summary:
			Returns the process's unique ID number
	*/
	virtual uint GetID() const = 0;

	/**
		Summary:
			Returns the ID of the session where this process is running
	*/
	virtual uint GetSessionID() const = 0;

	/**
		Summary:
			returns the point at which the process was started
	*/
	virtual std::chrono::time_point<std::chrono::system_clock> GetCreationTime() const = 0;

}; // class IProcess

inline IProcess::~IProcess() = default;

}; // namespace Platform
}; // namespace CainEngine