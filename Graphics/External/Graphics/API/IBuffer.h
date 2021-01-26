#pragma once

namespace CainEngine::Graphics::API {

class IBuffer : public IResource
{
	COMMON_DECLARE_INTERFACE(IBuffer);

public:
	// Main functionality

	virtual void* Map() = 0;

	virtual void Unmap() = 0;
};

inline IBuffer::~IBuffer() = default;

}