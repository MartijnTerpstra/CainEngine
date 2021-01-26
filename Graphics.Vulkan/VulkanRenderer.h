#pragma once

namespace CainEngine {
namespace Graphics {
namespace Vulkan {

class VulkanRenderer final : public Implementation::IRenderer
{
public:
	// ctor & dtor

	VulkanRenderer();
	~VulkanRenderer();

public:
	// IRendererImpl overrides

	uint32 ID() const override;

	void Init(flag<RendererInitFlags> initFlags) override;

	void Exit() override;

	void RenderFrame() override;

	void SetMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow) override;

	unique_ptr<VertexShader> CreateVertexShader(Implementation::CompiledShaderData&& shaderData) override;

	unique_ptr<PixelShader> CreatePixelShader(Implementation::CompiledShaderData&& shaderData) override;

	bool HasFeature(RendererFeature feature) const override;

private:
	// Member variables

}; // class VulkanRenderer

}; // namespace Vulkan
}; // namespace Graphics
}; // namespace CainEngine