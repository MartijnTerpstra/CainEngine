#pragma once

namespace CainEngine::Graphics::API {

class IResource
{
	COMMON_DECLARE_INTERFACE(IResource);

public:
	// Main functionality

	/**
		Summary:
			Returns the type of the resource
	*/
	virtual ResourceType type() const = 0;

	/**
		Summary:
			Returns the usage of the resource
	*/
	virtual Usage usage() const = 0;

	/**
		Summary:
			Returns the size of the resource
	*/
	virtual uint3 size() const = 0;
};

inline IResource::~IResource() = default;

} // namespace CainEngine::Graphics::API