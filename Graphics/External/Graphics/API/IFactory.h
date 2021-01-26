#pragma once

namespace CainEngine::Graphics::API {

class IRenderer;

class IFactory
{
public:
	inline IFactory() = default;
	inline virtual ~IFactory();

public:
	// Main functionality

	virtual VertexShader* CreateVertexShader(IRenderer* renderer, API::CompiledShaderData&& shaderData) = 0;

	virtual PixelShader* CreatePixelShader(IRenderer* renderer, API::CompiledShaderData&& shaderData) = 0;

	virtual VertexData* CreateVertexData(IRenderer* renderer, const VertexDataCreationInfo& creationInfo) = 0;

	/**
		Summary:
			Creates a buffer

		Params:
			- type: the type of resource
			- memory: a memory chunk containing the initial memory for the buffer
			- structSize: the size of the struct inside the buffer, only used for structured buffers and vertex buffers
			- usage: the read and writability on the CPU and GPU
	*/
	virtual shared_ptr<IBuffer> CreateBuffer(IRenderer* renderer, ResourceType type, size_t dataSize, memory_view memory = memory_view(nullptr, 0),
		uint structSize = 0, Usage usage = Usage::Default) = 0;

	/**
		Summary:
			Creates a 2D image

		Params:
			- type: the type of texture
			- size: the width and height of the texture, in pixels
			- format: the pixel format of the texture,
			- bindFlags: the pipeline stages this texture will be bound to
			- usage: the read and writability on the CPU and GPU
			- arraySize: the amount of textures in the texture array, use 1 for non-arrays
			- mipLevels: the amount of mip levels to create, use GRAPHICS_CORE_GENERATE_MAX_MIPS to let the api generate the maximal amount
	*/
	virtual pair<int32_t, API::ITexture*> CreateTexture(IRenderer* renderer, ResourceType type, const uint3& size,
		PixelFormat format = PixelFormat::Default,
		flag<BindFlags> bindFlags = BindFlags::ShaderResource,
		Usage usage = Usage::Default,
		uint arraySize = 1,
		uint mipLevels = 1) = 0;

	/**
		Summary:
			Creates a 2D image with inital pixel data

		Params:
			- type: the type of texture
			- size: the width, height and depth of the texture, in pixels
			- initialData: the pixel data the texture is initialized with
			- format: the pixel format of the texture,
			- bindFlags: the pipeline stages this texture will be bound to
			- usage: the read and writability on the CPU and GPU
			- arraySize: the amount of textures in the texture array, use 1 for non-arrays
			- mipLevels: the amount of mip levels to create, use GRAPHICS_CORE_GENERATE_MAX_MIPS to let the api generate the maximal amount
	*/
	virtual pair<int32_t, API::ITexture*> CreateTexture(IRenderer* renderer, ResourceType type, const uint3& size,
		array_view<API::PixelData> initialData,
		PixelFormat format = PixelFormat::Default,
		flag<BindFlags> bindFlags = BindFlags::ShaderResource,
		Usage usage = Usage::Default,
		uint arraySize = 1,
		uint mipLevels = 1) = 0;

}; // IFactory

inline IFactory::~IFactory() = default;

};