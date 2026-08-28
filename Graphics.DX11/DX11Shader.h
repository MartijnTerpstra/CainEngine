#pragma once

namespace CainEngine::Graphics::API {

class VertexShader
{
public:
	// ctor & dtor
	VertexShader(ID3D11Device* device, API::ShaderReflectionData&& shaderData,
		std::vector<uint8_t>&& byteCode, uint64_t inputRegisterHash);
	~VertexShader();

public:
	// Main functionality

	ID3D11VertexShader* GetShader() const;

	memory_view ByteCode() const;

	uint64_t InputRegisterHash() const;

private:
	com_ptr<ID3D11VertexShader> m_vertexShader;
	const std::vector<uint8_t> m_byteCode;
	const uint64_t m_inputRegisterHash;
	const API::ShaderReflectionData m_reflection;
};

class PixelShader
{
public:
	// ctor & dtor
	PixelShader(ID3D11Device* device, API::CompiledShaderData&& shaderData,
		const std::vector<uint8_t>& byteCode);
	~PixelShader();

public:
	// Main functionality

	ID3D11PixelShader* GetShader() const;

private:
	com_ptr<ID3D11PixelShader> m_pixelShader;
};

} // namespace CainEngine::Graphics::API