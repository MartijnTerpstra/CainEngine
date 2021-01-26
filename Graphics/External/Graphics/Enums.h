#pragma once

namespace CainEngine {
namespace Graphics {

#define GRAPHICS_CORE_MAXIMUM_SHADER_STRLEN		(63)
#define GRAPHICS_CORE_MAX_SWAPCHAIN_BACKBUFFERS	(16)
#define GRAPHICS_CORE_MAX_RENDER_TARGETS		(8)
#define GRAPHICS_CORE_MAX_VIEWPORTS				(16)
#define GRAPHICS_CORE_MAX_VERTEX_BUFFERS		(16)
#define GRAPHICS_CORE_MAX_RESOURCE_GROUPS		(24)

#define GRAPHICS_CORE_GENERATE_MAX_MIPS (~0U)

/**
	Summary:
		Triangle culling setting
*/
enum class CullingMode : byte
{
	/** do not use culling */
	None = 1,

	/** do not render front-facing triangles */
	Front,

	/** do not render back-facing triangles */
	Back,

	/** The default value */
	Default = Back,

}; // enum class CullingMode

/**
	Summary:
		Triangle filling setting
*/
enum class FillingMode : byte
{
	/** Show only the wireframes of the triangles */
	Wireframe = 1,

	/** Show the entire triangle */
	Solid,

	/** The default value */
	Default = Solid,

}; // enum class FillingMode

enum class PrimitiveTopology : byte
{
	/** A 3-vertex primitive list */
	TriangleList = 1,

	/** A 2-vertex primitive list */
	LineList,

	/** A 1-vertex primitive list */
	PointList,
};

/**
	Summary:
		Texture sampling setting
*/
enum class TextureSamplingMode : byte
{
	/** Good for when textures match the resolution perfectly */
	Nearest = 1,

	/** Good for textures when resolution somewhat matches the back buffer resolution */
	Bilinear,

	/** Good for rendering resolution independent */
	Trilinear,

	/** Good for rendering a texture with any orientation */
	Anisotropic4x,

	/** Good for rendering a texture with any orientation, with higher detail */
	Anisotropic8x,

	/** Good for rendering a texture with any orientation, with highest detail */
	Anisotropic16x,

	/** The default value */
	Default = Trilinear,

}; // enum class TextureSamplingMode

/**
	Summary:
		The way the texture read
*/
enum class TextureAddressMode : byte
{
	/** Wraps around the texture uv address */
	Wrap = 1,

	/** Clams the texture uv address to the borders */
	Clamp,

	/** Mirrors the texture uv address outside the borders */
	Mirror,

	/** The default value */
	Default = Clamp,

}; // enum class TextureaddressMode

/**
	Summary:
		The pixel format of the texture
*/
enum class PixelFormat : byte
{
	/** RGBA 8 bits per pixel */
	UnormRGBA8 = 1,

	/** BGRA 8 bits per pixel */
	UnormBGRA8,

	/** RGBA 16 bits per pixel */
	UnormRGBA16,

	/** RGBA 32 bit floats per pixel */
	FloatRGBA32,

	/** single 8 bit value */
	Unorm8,

	/** single 16 bit depth value */
	DepthUnorm16,

	/** 24 bit depth with 8 bit stencil */
	DepthUnorm24Stencil8,

	/** 32 bit float */
	DepthFloat32,

	/** BC1: r5g5b5a1 */
	BlockCompression1,

	/** BC2: r5g5b5a4 */
	BlockCompression2,

	/** BC3: r5g5b5a8 */
	BlockCompression3,

	/** BC4: single 8 bit value */
	BlockCompression4,

	/** BC4: r8g8 */
	BlockCompression5,

	/** BC7: r4g4b4a0 - r7g7b7b8 adaptive compression */
	BlockCompression7,
	
	/** RGBA 16 bit floats per pixel */
	FloatRGBA16,


	/** The default value */
	Default = UnormRGBA8,

	/** The default value for depth buffers */
	DefaultDepth = DepthUnorm24Stencil8,

}; // enum class PixelFormat

/**
	Summary:
		The type of shader
*/
enum class ShaderType : byte
{
	/** Vertex shader */
	Vertex = 1,

	/** Pixel/Fragment shader */
	Pixel,

	/** Compute shader */
	Compute,

	/** Geometry shader */
	Geometry,

	/** Hull shader */
	Hull,

	/** Domain shader */
	Domain,
};

/**
	Summary:
		The type of resource
*/
enum class ResourceType : byte
{
	/** A buffer containing vertex data */
	VertexBuffer = 1,

	/** A buffer containing index data */
	IndexBuffer,

	/** A buffer to be used in the shader, with a constant size and layout */
	ConstantBuffer,

	/** A list of bytes to be used in the shader */
	ByteAddressBuffer,

	/** A dynamic array of a certain type to be used in the shader */
	StructuredBuffer,

	/** A 1 dimensional texture */
	Tex1D,

	/** A 2 dimensional texture */
	Tex2D,

	/** A 6 sided texture cube */
	TexCube,

	/** A 3 dimensional texture */
	Tex3D,

}; // enum class ResourceType

/**
	Summary:
		The types of vertex variables
*/
enum class VertexBufferVariableType : byte
{
	Position = 1,
	Normal,
	Tangent,
	BiTangent,
	DiffuseUV,
	SpecularUV,

};

/**
	Summary:
		Option flags to pass to the renderer on creation
*/
enum class RendererInitFlags : byte
{
	/** Graphics debugging */
	ApiDebug,

	/** Render */
	PipelineDebug,
};

enum class RendererFeature : byte
{
	/** Allows shader loading in a background thread */
	AsyncShaderLoading = 1,
	MultiThreadedCommandLists
};

}; // namespace Graphics
}; // namespace CainEngine