#pragma once

namespace CainEngine::Graphics::API {

/**
	Summary:
		Types of variables available in shaders
*/
enum class ShaderVariableType : byte
{
	/** A 32 bit IEEE 754 float value */
	Float,

	/** A 32 bit two's complement signed integer value */
	Int32,

	/** A 32 bit unsigned integer value */
	Uint32,
};

/**
	Summary:
		Shader semantic names
*/
enum class ShaderSemanticName : byte
{
	/** The POSITION semantic */
	Position,

	/** The TEXCOORD semantic */
	Texcoord,

	/** The NORMAL */
	Normal,

}; // enum class ShaderSemanticName

/**
	Summary:
		The CPU<->GPU read/write behaviour of the resource
*/
enum class Usage : byte
{
	/** GPU readonly */
	Constant = 1,

	/** GPU read/write */
	GpuReadWrite,

	/** GPU read only, CPU write only */
	Dynamic,

	/** GPU copy only, CPU read only */
	Staging,

	/** The default value */
	Default = Constant,

}; // enum class Usage

/**
	Summary:
		The binding of the texture to the graphics pipeline
*/
enum class BindFlags : byte
{
	/** This object will by used by shaders */
	ShaderResource,

	/** This object will by used as a target to be rendered to */
	RenderTarget,

	/** This object will by used as a depth/stencil buffer */
	DepthStencil,
};

}