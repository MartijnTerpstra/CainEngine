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
	virtual ResourceType Type() const = 0;

	/**
		Summary:
			Returns the usage of the resource
	*/
	virtual Usage Usage() const = 0;

	/**
		Summary:
			Returns the size of the resource
	*/
	virtual uint3 Size() const = 0;

};

inline IResource::~IResource() = default;

}