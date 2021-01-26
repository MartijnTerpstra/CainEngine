#include "Precomp.h"

#include "DX11RenderContext.h"

#include "InputLayoutResolver.h"
#include "DX11Texture.h"
#include "DX11Factory.h"
#include "DX11Renderer.h"

#include "DX11Shader.h"
#include "VertexData.h"

using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX11;

DX11RenderContext::DX11RenderContext(DX11Renderer* renderer, ID3D11DeviceContext* context)
	: m_context(context),
	m_renderer(renderer)
{
}

DX11RenderContext::~DX11RenderContext()
{
}

void DX11RenderContext::SetViewport(const Viewport& vp, float depthMin, float depthMax)
{
	D3D11_VIEWPORT v;
	v.TopLeftX = vp.x;
	v.TopLeftY = vp.y;
	v.Width = vp.width;
	v.Height = vp.height;
	v.MinDepth = depthMin;
	v.MaxDepth = depthMax;

	m_context->RSSetViewports(1, &v);
}

void DX11RenderContext::SetVertexData(const API::VertexData& vertexData)
{
	if (m_vertexData == &vertexData)
		return;

	m_vertexData = &vertexData;

	inlined_vector<ID3D11Buffer*, 8> buffers(vertexData.vertexBuffers.size());

	for (size_t i = 0; i < buffers.size(); ++i)
	{
		buffers[i] = vertexData.vertexBuffers[i].get();
	}

	const UINT* strides = &vertexData.stridesAndOffsets[0];
	const UINT* offsets = &vertexData.stridesAndOffsets[buffers.size()];

	m_context->IASetVertexBuffers(0, (UINT)buffers.size(), buffers.data(), strides, offsets);
	m_context->IASetPrimitiveTopology(vertexData.topology);
	m_context->IASetIndexBuffer(vertexData.indexBuffer.get(), vertexData.indexFormat, 0);
}

void DX11RenderContext::SetVertexShader(API::VertexShader* shader)
{
	if (shader == m_vertexShader)
		return; // do nothing

	m_context->VSSetShader(shader->GetShader(), nullptr, 0);

	m_context->IASetInputLayout(m_renderer->GetOrCreateInputLayout(shader, m_vertexData->vertexLayoutHash, m_vertexData->vertexLayout));
}

void DX11RenderContext::SetPixelShader(API::PixelShader* shader)
{
	m_context->PSSetShader(shader->GetShader(), nullptr, 0);
}

void DX11RenderContext::Draw(uint32_t vertexCount, uint32_t startOffset)
{
	m_context->Draw(vertexCount, startOffset);
}

void DX11RenderContext::DrawIndexed(uint32_t indexCount, uint32_t startOffset)
{
	m_context->DrawIndexed(indexCount, startOffset, 0);
}