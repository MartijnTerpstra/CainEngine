#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;

void Renderer::BuildFrameData(EntitySystem::Scene& scene, const Graphics::Viewport& viewport)
{
}

void Renderer::RenderFrame(EntitySystem::Scene& scene, CameraManager& cameras, ModelManager& models, const optional<Viewport>& vp)
{
	++m_frameIndex;

	auto viewport = vp.value_or(GetViewport());

	BuildFrameData(scene, viewport);

	m_renderer->RenderFrame([&](API::IRenderContext* context)
	{
		context->SetViewport(viewport);

		/*

		m_context->ClearRenderTargetView(m_backBufferRTV.get(), float4::zero.data());

		m_context->OMSetRenderTargets(1, &m_backBufferRTV, null);

		*/

		for (auto& modelData : models.GetModels())
		{
			const auto& model = modelData.model;

			context->SetVertexData(model.VertexData());

			for (auto subMesh : model.SubMeshes())
			{
				const auto material = models.GetMaterial(subMesh.materialIndex);

				context->SetVertexShader(material->VertexShader());
				context->SetPixelShader(material->PixelShader());

				context->DrawIndexed(subMesh.indexCount, subMesh.indexOffset);
			}

		}
	});

}