#pragma once

namespace CainEngine {
namespace Graphics {

struct Viewport
{
	Viewport(float x, float y,
		float width, float height)
		: x(x)
		, y(y)
		, width(width)
		, height(height)
	{

	}

	Viewport(const float2& xy,
		const float2& size)
		: x(xy.x)
		, y(xy.y)
		, width(size.x)
		, height(size.y)
	{

	}

	float x, y, width, height;
};

struct VertexDataCreationInfo
{
	vector<float3> positions;
	vector<float3> normals;
	vector<float2> uvs;
	vector<uint32_t> indices;
	PrimitiveTopology topology = PrimitiveTopology::TriangleList;
};

/**
	Summary:
		Information about a resource in the shader
*/
struct ShaderResourceInfo
{
	ShaderResourceInfo() = default;
	explicit ShaderResourceInfo(uint64_t nameHash,
		ResourceType type,
		uint slot,
		uint space = 0)
		: nameHash(nameHash),
		type(type),
		slot(slot),
		space(space)
	{
	}

	/**
		Summary:
			The name the resource has in the shader
	*/
	uint64_t nameHash;

	/**
		Summary:
			The type of resource
	*/
	ResourceType type;

	/**
		Summary:
			The slot this resource is mapped to
	*/
	uint slot;

	/**
		Summary:
			The space this resource is occupying
	*/
	uint space = 0;

}; // struct ShaderResourceInfo

/**
	Summary:
		Information about a buffer in the shader
*/
struct ShaderBufferInfo : ShaderResourceInfo
{
	ShaderBufferInfo() = default;
	explicit ShaderBufferInfo(uint64_t nameHash,
		ResourceType type,
		uint slot,
		size_t dataSize = 0,
		uint space = 0)
		: ShaderResourceInfo(nameHash, type, slot, space),
		dataSize(dataSize)
	{
	}

	/**
		Summary:
			The size of the buffer data

		Notes:
			This is only used for constant buffers
	*/
	size_t dataSize = 0;

}; // struct ShaderBufferInfo

/**
	Summary:
		Information about a texture in the shader
*/
struct ShaderTextureInfo : ShaderResourceInfo
{
	ShaderTextureInfo() = default;
	explicit ShaderTextureInfo(uint64_t nameHash,
		ResourceType type,
		uint slot,
		bool isArray = false,
		uint space = 0)
		: ShaderResourceInfo(nameHash, type, slot, space)
	{
	}

	bool IsArray = false;

}; // struct ShaderTextureInfo

/**
	Summary:
		Possible display mode
*/
struct SwapChainDisplayMode
{
	SwapChainDisplayMode(uint32_t width,
		uint32_t height,
		uint32_t refreshRate)
		: resolution(width, height),
		refreshRate(refreshRate)
	{
	}

	uint2 resolution;

	/** The refresh rate in Hz */
	uint32_t refreshRate;
};

struct SwapChainCreationSettings
{
	SwapChainCreationSettings(PixelFormat pixelFormat,
		uint32_t refreshRate,
		bool vSync,
		uint32_t multiSamplingCount = 1,
		uint32_t preferredOutput = 0)
		: pixelFormat(pixelFormat),
		refreshRate(refreshRate),
		vSync(vSync),
		multiSamplingCount(multiSamplingCount),
		preferredOutput(preferredOutput)
	{
	}

	PixelFormat pixelFormat;
	bool vSync;
	uint32_t refreshRate;
	uint32_t multiSamplingCount;
	uint32_t preferredOutput;
};

struct OutputDisplay
{
	OutputDisplay(uint32_t index,
		string name)
		: index(index),
		name(name)
	{
	}

	uint32_t index;
	string name;
};

struct FullScreenState
{
	FullScreenState(bool isFullScreen,
		uint32_t outputIndex)
		: isFullScreen(isFullScreen),
		outputIndex(outputIndex)
	{
	}

	bool isFullScreen;
	uint32_t outputIndex;
};

struct ShaderMetaData
{
};

struct VertexShaderMetaData : ShaderMetaData
{
};

}; // namespace Graphics
}; // namespace CainEngine