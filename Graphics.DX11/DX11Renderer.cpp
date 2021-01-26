#include "Precomp.h"

#include "InputLayoutResolver.h"
#include "DX11Texture.h"
#include "DX11Factory.h"
#include "DX11Renderer.h"

#include "EnumConverter.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX11;

unique_ptr<API::IRenderer> DX11::CreateInstance()
{
	return make_unique<DX11::DX11Renderer>();
}

DX11Renderer::DX11Renderer()
{
	m_d3d11 = LoadLibraryA("D3D11.dll");
}

DX11Renderer::~DX11Renderer()
{
	FreeLibrary(m_d3d11);
}

ID3D11Device* DX11Renderer::D3DDevice()
{
	return m_device.get();
}

void DX11Renderer::AddVertexShaderInputRegisters(uint64_t inputRegisterHash, array_view<API::ShaderRegisterInfo> inputRegisters)
{
	m_inputLayoutResolver.AddVertexShaderInputRegisters(inputRegisterHash, inputRegisters);
}

ID3D11InputLayout* DX11Renderer::GetOrCreateInputLayout(API::VertexShader* vs, uint64_t vertexLayoutHash, mst::array_view<API::VertexBufferDesc> vertexLayout)
{
	return m_inputLayoutResolver.Resolve(m_device.get(), vs, vertexLayoutHash, vertexLayout);
}

void DX11Renderer::RemoveSwapChain()
{
	Flush();

	// Release old window handles
	m_context->OMSetRenderTargets(0, nullptr, nullptr);
	m_backBufferRTV.reset();
	m_backbuffer.reset();
	m_swapChain.reset();
}

void DX11Renderer::CreateSwapChain(DXGI_SWAP_CHAIN_DESC& desc)
{
	com_ptr<IDXGIFactory> factory;
	CHECK_HRESULT(m_adapter->GetParent(MST_IID_PPV_ARGS(factory)));

	CHECK_HRESULT(factory->CreateSwapChain(m_device.get(), &desc, mst::initialize(m_swapChain)));

	CHECK_HRESULT(m_swapChain->GetBuffer(0, MST_IID_PPV_ARGS(m_backbuffer)));

	UpdateRTV(MultiSamplingCount());
}

void CainEngine::Graphics::DX11::DX11Renderer::UpdateRTV(uint32_t multiSamplingCount)
{
	m_msBackBuffer.reset();

	if (multiSamplingCount == 1)
	{ // No multi sampling

		CHECK_HRESULT(m_device->CreateRenderTargetView(m_backbuffer.get(), nullptr, mst::initialize(m_backBufferRTV)));
	}
	else
	{
		D3D11_TEXTURE2D_DESC desc;

		m_backbuffer->GetDesc(&desc);

		UINT qualityLevels;
		CHECK_HRESULT(m_device->CheckMultisampleQualityLevels(desc.Format, multiSamplingCount, &qualityLevels));

		if (qualityLevels == 0)
			Common::FatalError("invalid argument: multiSamplingCount (%u), sampling count is not supported", multiSamplingCount);

		desc.SampleDesc.Count = multiSamplingCount;
		desc.SampleDesc.Quality = qualityLevels - 1;

		CHECK_HRESULT(m_device->CreateTexture2D(&desc, nullptr, mst::initialize(m_msBackBuffer)));

		CHECK_HRESULT(m_device->CreateRenderTargetView(m_msBackBuffer.get(), nullptr, mst::initialize(m_backBufferRTV)));
	}
}