#pragma once

namespace CainEngine::Graphics::API {

class VertexShader
{
public:
	// ctor & dtor
	VertexShader(ID3D11Device* device, API::CompiledShaderData&& shaderData, vector<byte>&& byteCode, uint64_t inputRegisterHash);
	~VertexShader();

public:
	// Main functionality

	ID3D11VertexShader* GetShader() const;

	memory_view ByteCode() const;

	uint64_t InputRegisterHash() const;

private:
	com_ptr<ID3D11VertexShader> m_vertexShader;
	const vector<byte> m_byteCode;
	const uint64_t m_inputRegisterHash;
	const API::ShaderReflectionData m_reflection;
};

class PixelShader
{
public:
	// ctor & dtor
	PixelShader(ID3D11Device* device, API::CompiledShaderData&& shaderData, const vector<byte>& byteCode);
	~PixelShader();

public:
	// Main functionality

	ID3D11PixelShader* GetShader() const;

private:
	com_ptr<ID3D11PixelShader> m_pixelShader;
};

}