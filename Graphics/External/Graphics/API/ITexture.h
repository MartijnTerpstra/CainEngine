#pragma once

namespace CainEngine::Graphics::API {

/**
	Summary:
		Interface of a 2D image, for use on the GPU
*/
class ITexture : public IResource
{
	COMMON_DECLARE_INTERFACE(ITexture);

public:
	// Main functionality

	/**
		Summary:
			Returns the format of the pixels in the texture
	*/
	virtual PixelFormat Format() const = 0;

	/**
		Summary:
			Returns array elements in this texture
	*/
	virtual uint ArraySize() const = 0;

	/**
		Summary:
			Returns the to what part of the pipeline the texture's will be bound
	*/
	virtual flag<BindFlags> BindFlags() const = 0;

}; // class ITexture

inline ITexture::~ITexture() = default;

}; // namespace Renderer