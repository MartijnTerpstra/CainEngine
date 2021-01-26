#pragma once

namespace CainEngine::Graphics::API {

constexpr uint32_t SHADER_MAGIC_NUMBER = Common::PackChars('c', 'e', 'c', 's');

/**
	Summary:
		Meta data of a compiled shader
*/
struct CompiledShaderMetaData
{
	stack_string<64> shaderName;
	ShaderType shaderType;
	uint64_t memoryOffset;
};

struct VertexBufferDesc
{
	VertexBufferDesc() = default;
	explicit VertexBufferDesc(ShaderSemanticName semanticName,
		uint32_t semanticIndex = 0,
		ShaderVariableType type = ShaderVariableType::Float,
		uint32_t elementCount = 1,
		uint32_t byteOffset = UINT_MAX,
		uint8_t vertexBufferIndex = 0)
		: semanticName(semanticName),
		semanticIndex(semanticIndex),
		type(type),
		elementCount(elementCount),
		byteOffset(byteOffset),
		vertexBufferIndex(vertexBufferIndex)
	{
	}

	/**
		Summary:
			The register semantic in the vertex shader
	*/
	ShaderSemanticName semanticName;

	/**
		Summary:
			The semantic index in the vertex shader
	*/
	uint32_t semanticIndex = 0;

	/**
		Summary:
			The type of variable as declared in the shader
	*/
	ShaderVariableType type = ShaderVariableType::Float;

	/**
		Summary:
			The amount of elements in the vector

		Range:
			[1, 4]
	*/
	uint32_t elementCount = 1;

	/**
		Summary:
			The offset in bytes from the base address of the vertex struct

		Range:
			[0, inf], use UINT_MAX to append to previous layout elements
	*/
	uint32_t byteOffset = UINT_MAX;

	/**
		Summary:
			The offset in bytes from the base address of the vertex struct

		Range:
			[0, inf], use UINT_MAX to append to previous layout elements
	*/
	uint8_t vertexBufferIndex = 0;

}; // struct VertexBufferVariable

struct VertexBufferData
{
	VertexBufferData() = default;
	VertexBufferData(memory_view vertexData,
		uint32_t strideInBytes,
		inlined_vector<VertexBufferDesc, 16> vertexLayout)
		: vertexData(vertexData),
		strideInBytes(strideInBytes),
		vertexLayout(std::move(vertexLayout))
	{
	}

	memory_view vertexData;
	uint32_t strideInBytes;
	inlined_vector<VertexBufferDesc, 16> vertexLayout;
};

/**
	Summary:
		Describes the input expected from the shader
*/
struct ShaderRegisterInfo
{
	ShaderRegisterInfo() = default;
	explicit ShaderRegisterInfo(uint32_t registerSlot,
		ShaderSemanticName semanticName,
		uint32_t semanticIndex = 0,
		ShaderVariableType variableType = ShaderVariableType::Float,
		uint32_t variableElementCount = 1,
		uint32_t registerSpace = 0)
		: registerSlot(registerSlot),
		semanticName(semanticName),
		semanticIndex(semanticIndex),
		variableType(variableType),
		variableElementCount(variableElementCount)
	{
	}

	/**
		Summary:
			The register slot in the shader
	*/
	uint32_t registerSlot = 0;

	/**
		Summary:
			Shader semantic name (POSITION, TEXCOORD)
	*/
	ShaderSemanticName semanticName;

	/**
		Summary:
			Shader semantic index (TEXCOORD0, TEXCOORD1)
	*/
	uint32_t semanticIndex = 0;

	/**
		Summary:
			The type of variable as declared in the shader
	*/
	ShaderVariableType variableType = ShaderVariableType::Float;

	/**
		Summary:
			The amount of elements in the vector

		Range:
			[1, 4]
	*/
	uint32_t variableElementCount = 1;

}; // struct ShaderRegisterInfo

/**
	Summary:
		Data required for shader reflection
*/
struct ShaderReflectionData
{
	vector<ShaderBufferInfo> buffers;
	vector<ShaderTextureInfo> textures;
	vector<ShaderRegisterInfo> inputRegisters;
	vector<ShaderRegisterInfo> outputRegisters;
};

/**
	Summary:
		Data of a compiled shader
*/
struct CompiledShaderData : ShaderReflectionData
{
	vector<byte> byteCode;
};

/**
	Summary:
		Data of a compiled shader
*/
struct PixelData
{
	uint32_t pitch;
	uint32_t size;
	const byte* pixels;
};

}