#pragma once

namespace CainEngine::Graphics::DX11 {

class DX11Renderer final : public API::IRenderer, private API::IDisplaySettings
{
public:
	// ctor & dtor

	DX11Renderer();
	~DX11Renderer();

public:
	// Main functionality
	
	ID3D11Device* d3DDevice();

	void addVertexShaderInputRegisters(uint64_t inputRegisterHash, array_view<API::ShaderRegisterInfo> inputRegisters);

	ID3D11InputLayout* getOrCreateInputLayout(API::VertexShader* vs, uint64_t vertexLayoutHash, mst::array_view<API::VertexBufferDesc> vertexLayout);

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
	// Internal functionality

	void removeSwapChain();
	void createSwapChain(DXGI_SWAP_CHAIN_DESC& desc);
	void ensureFullscreen(bool fullscreen, IDXGIOutput* output);
	void updateRtv(uint32_t multiSamplingCount);

private:
	// Member variables

	com_ptr<ID3D11DeviceContext> m_context;
	com_ptr<ID3D11Device> m_device;
	com_ptr<ID3D11RenderTargetView> m_backBufferRTV;

	com_ptr<IDXGISwapChain> m_swapChain;
	com_ptr<ID3D11Texture2D> m_backbuffer, m_msBackBuffer;
	Common::RefPtr<Platform::Win32::IWin32Window> m_mainWindow;
	com_ptr<IDXGIAdapter> m_adapter;

	InputLayoutResolver m_inputLayoutResolver;

	DX11Factory m_factory;

	bool m_vSync = false;

	HMODULE m_d3d11;
};

};