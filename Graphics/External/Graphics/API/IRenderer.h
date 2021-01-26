#pragma once

namespace CainEngine::Graphics::API {

class IRenderer
{
public:
	inline IRenderer() = default;
	inline virtual ~IRenderer();

public:
	// Main functionality

	[[nodiscard]] virtual uint32_t ID() const noexcept = 0;

	[[nodiscard]] virtual string ShortName() const noexcept = 0;

	[[nodiscard]] virtual string Name() const noexcept = 0;

	virtual void Init(flag<RendererInitFlags> initFlags) = 0;

	virtual void Exit() = 0;

	virtual void RenderFrame(std::function<void(API::IRenderContext*)> onRender) = 0;

	virtual void Flush() = 0;

	virtual void SetMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow, const optional<SwapChainCreationSettings>& creationSettings) = 0;

	virtual void HandleWindowResize() = 0;

	[[nodiscard]] virtual bool HasFeature(RendererFeature feature) const = 0;

	[[nodiscard]] virtual uint2 GetBackBufferSize() const noexcept = 0;

	[[nodiscard]] virtual IDisplaySettings& DisplaySettings() noexcept = 0;

	[[nodiscard]] virtual API::IFactory* GetFactory() noexcept = 0;
};

inline IRenderer::~IRenderer() = default;

};