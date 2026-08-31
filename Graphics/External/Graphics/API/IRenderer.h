#pragma once

namespace CainEngine::Graphics::API {

class IRenderer
{
public:
	inline IRenderer() = default;
	inline virtual ~IRenderer();

public:
	// Main functionality

	[[nodiscard]] virtual uint32_t id() const noexcept = 0;

	[[nodiscard]] virtual std::string shortName() const noexcept = 0;

	[[nodiscard]] virtual std::string name() const noexcept = 0;

	virtual void init(flag<RendererInitFlags> initFlags) = 0;

	virtual void exit() = 0;

	virtual void renderFrame(std::function<void(API::IRenderContext*)> onRender) = 0;

	virtual void flush() = 0;

	virtual void setMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow, const std::optional<SwapChainCreationSettings>& creationSettings) = 0;

	virtual void handleWindowResize() = 0;

	[[nodiscard]] virtual bool hasFeature(RendererFeature feature) const = 0;

	[[nodiscard]] virtual uint2 getBackBufferSize() const noexcept = 0;

	[[nodiscard]] virtual IDisplaySettings& displaySettings() noexcept = 0;

	[[nodiscard]] virtual API::IFactory* getFactory() noexcept = 0;
};

inline IRenderer::~IRenderer() = default;

};