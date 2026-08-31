#pragma once

namespace CainEngine {
namespace Graphics {
namespace DX12 {

// NOTE: init()/renderFrame()/exit()/setMainWindow() are a real (if minimal)
// D3D12 device/swapchain implementation. Everything else required by the
// current Graphics::API::IRenderer/IDisplaySettings interfaces that this
// class predates - shortName()/name()/flush()/handleWindowResize()/
// getBackBufferSize()/displaySettings()/getFactory() plus the whole
// IDisplaySettings surface - is a compile-only stub (reports "not
// implemented"), same treatment as Graphics.Vulkan's VulkanRenderer.
class DX12Renderer final : public API::IRenderer, private API::IDisplaySettings
{
public:
	// ctor & dtor

	DX12Renderer();
	~DX12Renderer();

public:
	// IRenderer overrides

	uint32_t id() const noexcept override;

	std::string shortName() const noexcept override;

	std::string name() const noexcept override;

	void init(flag<RendererInitFlags> initFlags) override;

	void exit() override;

	void renderFrame(std::function<void(API::IRenderContext*)> onRender) override;

	void flush() override;

	void setMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow, const std::optional<SwapChainCreationSettings>& creationSettings) override;

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
	std::vector<SwapChainDisplayMode> supportedDisplayModes(uint32_t outputIndex, PixelFormat format) const override;
	std::vector<uint32_t> supportedMultiSamplingCounts(PixelFormat format) const override;

private:
	// Member variables

	uint32_t m_renderIndex = 0;

	D3D_FEATURE_LEVEL m_featureLvl;
	com_ptr<IDXGIFactory6> m_factory;
	com_ptr<IDXGIAdapter> m_adapter;
	com_ptr<ID3D12Device> m_device;
	com_ptr<ID3D12CommandQueue> m_queue;
	com_ptr<IDXGISwapChain3> m_swapChain;
	com_ptr<ID3D12Resource> m_backbuffers[2];
	com_ptr<ID3D12CommandAllocator> m_allocators[2];
	com_ptr<ID3D12GraphicsCommandList> m_commandLists[2];
	Event m_commandQueueCompletedEvents[2];
	Event m_presentReadyEvents[2];

}; // class DX12Renderer

}; // namespace DX12
}; // namespace Graphics
}; // namespace CainEngine
