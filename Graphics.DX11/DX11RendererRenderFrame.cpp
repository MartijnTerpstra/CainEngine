#include "Precomp.h"

#include "InputLayoutResolver.h"
#include "DX11Texture.h"
#include "DX11Factory.h"
#include "DX11Renderer.h"

#include "DX11Shader.h"
#include "DX11RenderContext.h"

#include "EnumConverter.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX11;

void DX11Renderer::RenderFrame(std::function<void(API::IRenderContext*)> onRender)
{
	COMMON_CALLSTACK_CALL;

	m_context->ClearRenderTargetView(m_backBufferRTV.get(), float4::zero.data());

	m_context->OMSetRenderTargets(1, &m_backBufferRTV, nullptr);

	DX11RenderContext renderContext{ this, m_context.get() };

	onRender(&renderContext);

	if (m_msBackBuffer)
	{
		D3D11_TEXTURE2D_DESC desc;
		m_backbuffer->GetDesc(&desc);
		m_context->ResolveSubresource(m_backbuffer.get(), 0, m_msBackBuffer.get(), 0, desc.Format);
	}

	m_swapChain->Present(m_vSync ? 1 : 0, 0);
}