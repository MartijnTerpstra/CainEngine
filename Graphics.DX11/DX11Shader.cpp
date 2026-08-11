#include "Precomp.h"

#include "DX11Shader.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX11;

API::VertexShader::VertexShader(
	ID3D11Device* device, API::ShaderReflectionData&& shaderData, std::vector<byte>&& byteCode, uint64_t inputRegisterHash)
	: m_byteCode(std::move(byteCode))
	, m_inputRegisterHash(inputRegisterHash)
	, m_reflection(std::move(shaderData))
{
	CHECK_HRESULT(device->CreateVertexShader(
		m_byteCode.data(), m_byteCode.size(), nullptr, mst::initialize(m_vertexShader)));
}

API::VertexShader::~VertexShader()
{ }

ID3D11VertexShader* API::VertexShader::GetShader() const
{
	return m_vertexShader.get();
}

memory_view API::VertexShader::ByteCode() const
{
	return memory_view(m_byteCode);
}

uint64_t API::VertexShader::InputRegisterHash() const
{
	return m_inputRegisterHash;
}

API::PixelShader::PixelShader(
	ID3D11Device* device, API::CompiledShaderData&& shaderData, const std::vector<byte>& byteCode)
{
	CHECK_HRESULT(device->CreatePixelShader(
		byteCode.data(), byteCode.size(), nullptr, mst::initialize(m_pixelShader)));
}

API::PixelShader::~PixelShader()
{ }

ID3D11PixelShader* API::PixelShader::GetShader() const
{
	return m_pixelShader.get();
}