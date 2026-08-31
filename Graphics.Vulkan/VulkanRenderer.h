#pragma once

namespace CainEngine::Graphics::Vulkan {

// NOTE: This is a compile-only stub. There is no actual Vulkan instance/
// device/swapchain setup behind it yet (see Linux/CreateSwapChain.cpp and
// Win32/CreateSwapChain.cpp, both currently empty) - every override here
// just reports "not implemented" rather than doing anything. It exists so
// Graphics.Vulkan builds against the current Graphics::API::IRenderer/
// IDisplaySettings interfaces; it is not a working renderer.
class VulkanRenderer final : public API::IRenderer, private API::IDisplaySettings
{
public:
	// ctor & dtor

	VulkanRenderer();
	~VulkanRenderer();

public:
	// IRenderer overrides

	uint32_t id() const noexcept override;

	std::string shortName() const noexcept override;

	std::string name() const noexcept override;

	void init(flag<RendererInitFlags> initFlags) override;

	void exit() override;

	void renderFrame(std::function<void(API::IRenderContext*)> onRender) override;

	void flush() override;

	void setMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow,
		const std::optional<SwapChainCreationSettings>& creationSettings) override;

	void handleWindowResize() override;

	bool hasFeature(RendererFeature feature) const override;

	uint2 getBackBufferSize() const noexcept override;

	IDisplaySettings& displaySettings() noexcept override;

	API::IFactory* getFactory() noexcept override;

private:
	// IDisplaySettings overrides

	FullScreenState fullScreen() const override;
	void setFullScreen(bool fullScreen, uint32_t outputIndex) override;
	uint32_t multiSamplingCount() const override;
	void setMultiSamplingCount(uint32_t count) override;
	bool vSync() const override;
	void setVSync(bool vSync) override;
	PixelFormat format() const override;
	void setFormat(PixelFormat format) override;
	SwapChainDisplayMode displayMode() const override;
	void setDisplayMode(const SwapChainDisplayMode& mode) override;
	std::vector<OutputDisplay> supportedOutputs() const override;
	std::vector<PixelFormat> supportedPixelFormats(uint32_t outputIndex) const override;
	std::vector<SwapChainDisplayMode> supportedDisplayModes(
		uint32_t outputIndex, PixelFormat format) const override;
	std::vector<uint32_t> supportedMultiSamplingCounts(PixelFormat format) const override;
};

}; // namespace CainEngine::Graphics::Vulkan
