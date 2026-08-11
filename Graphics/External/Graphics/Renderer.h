#pragma once

namespace CainEngine {
namespace Graphics {

class Renderer
{
public:
	// ctor & dtor

	Renderer(std::unique_ptr<API::IRenderer> renderer);
	~Renderer();

public:
	// Main functionality

	/**
		Get the ID of the graphics API
	*/
	uint32_t ID() const;

	std::string ShortName() const;

	std::string Name() const;

	void Init(flag<RendererInitFlags> initFlags);

	void Exit();

	void RenderFrame(EntitySystem::Scene& scene, CameraManager& cameras, ModelManager& models, const std::optional<Graphics::Viewport>& viewport);

	void Flush();

	void SetMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow, const std::optional<SwapChainCreationSettings>& creationSettings);

	void HandleWindowResize();

	bool HasFeature(RendererFeature feature) const;

	API::IDisplaySettings& DisplaySettings();

	API::IRenderer* Implementation();

private:
	// Internal functionality

	Viewport GetViewport() const;

	void BuildFrameData(EntitySystem::Scene& scene, const Graphics::Viewport& viewport);

private:

	const std::unique_ptr<API::IRenderer> m_renderer;
	uint32_t m_frameIndex = 0;

};

}; // namespace Graphics
}; // namespace CainEngine