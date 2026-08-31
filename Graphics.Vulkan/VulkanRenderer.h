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

	uint32 id() const override;

	void init(flag<RendererInitFlags> initFlags) override;

	void exit() override;

	void renderFrame() override;

	void setMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow) override;

	unique_ptr<VertexShader> createVertexShader(Implementation::CompiledShaderData&& shaderData) override;

	unique_ptr<PixelShader> createPixelShader(Implementation::CompiledShaderData&& shaderData) override;

	bool hasFeature(RendererFeature feature) const override;

private:
	// Member variables

}; // class VulkanRenderer

}; // namespace Vulkan
}; // namespace Graphics
}; // namespace CainEngine