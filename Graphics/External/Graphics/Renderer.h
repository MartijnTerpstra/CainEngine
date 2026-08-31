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
	uint32_t id() const;

	std::string shortName() const;

	std::string name() const;

	void init(flag<RendererInitFlags> initFlags);

	void exit();

	void renderFrame(EntitySystem::Scene& scene, CameraManager& cameras, ModelManager& models,
		const std::optional<Graphics::Viewport>& viewport);

	void flush();

	void setMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow,
		const std::optional<SwapChainCreationSettings>& creationSettings);

	void handleWindowResize();

	bool hasFeature(RendererFeature feature) const;

	API::IDisplaySettings& displaySettings();

	API::IRenderer* implementation();

private:
	// Internal functionality

	Viewport getViewport() const;

	void buildFrameData(EntitySystem::Scene& scene, const Graphics::Viewport& viewport);

private:
	const std::unique_ptr<API::IRenderer> m_renderer;
	uint32_t m_frameIndex = 0;
};

}; // namespace Graphics
}; // namespace CainEngine