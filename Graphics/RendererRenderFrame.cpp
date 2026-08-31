#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;

void Renderer::buildFrameData(EntitySystem::Scene& scene, const Graphics::Viewport& viewport)
{ }

void Renderer::renderFrame(EntitySystem::Scene& scene, CameraManager& cameras, ModelManager& models,
	const std::optional<Viewport>& vp)
{
	++m_frameIndex;

	auto viewport = vp.value_or(getViewport());

	buildFrameData(scene, viewport);

	m_renderer->renderFrame([&](API::IRenderContext* context) {
		context->setViewport(viewport);

		/*

		m_context->ClearRenderTargetView(m_backBufferRTV.get(), float4::zero.data());

		m_context->OMSetRenderTargets(1, &m_backBufferRTV, null);

		*/

		for(auto& modelData : models.getModels())
		{
			const auto& model = modelData.model;

			context->setVertexData(model.vertexData());

			for(auto subMesh : model.subMeshes())
			{
				const auto material = models.getMaterial(subMesh.materialIndex);

				context->setVertexShader(material->vertexShader());
				context->setPixelShader(material->pixelShader());

				context->drawIndexed(subMesh.indexCount, subMesh.indexOffset);
			}
		}
	});
}
