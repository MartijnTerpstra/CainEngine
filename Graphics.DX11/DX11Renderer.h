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
	
	ID3D11Device* D3DDevice();

	void AddVertexShaderInputRegisters(uint64_t inputRegisterHash, array_view<API::ShaderRegisterInfo> inputRegisters);

	ID3D11InputLayout* GetOrCreateInputLayout(API::VertexShader* vs, uint64_t vertexLayoutHash, mst::array_view<API::VertexBufferDesc> vertexLayout);

public:
	// IRenderer overrides

	uint32_t ID() const noexcept override;

	string ShortName() const noexcept override;

	string Name() const noexcept override;

	void Init(flag<RendererInitFlags> initFlags) override;

	void Exit() override;

	void RenderFrame(std::function<void(API::IRenderContext*)> onRender) override;

	void Flush() override;

	void SetMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow, const optional<SwapChainCreationSettings>& creationSettings) override;

	void HandleWindowResize() override;

	bool HasFeature(RendererFeature feature) const override;

	uint2 GetBackBufferSize() const noexcept override;

	IDisplaySettings& DisplaySettings() noexcept override;

	API::IFactory* GetFactory() noexcept override;

private:
	// IDisplaySettings overrides
	
	FullScreenState FullScreen() const override;
	void SetFullScreen(bool fullScreen, uint32_t outputIndex) override;
	uint32_t MultiSamplingCount() const override;
	void SetMultiSamplingCount(uint32_t count) override;
	bool VSync() const override;
	void SetVSync(bool vSync) override;
	PixelFormat Format() const override;
	void SetFormat(PixelFormat format) override;
	SwapChainDisplayMode DisplayMode() const override;
	void SetDisplayMode(const SwapChainDisplayMode& mode) override;
	vector<OutputDisplay> SupportedOutputs() const override;
	vector<PixelFormat> SupportedPixelFormats(uint32_t outputIndex) const override;
	vector<SwapChainDisplayMode> SupportedDisplayModes(uint32_t outputIndex, PixelFormat format) const override;
	vector<uint32_t> SupportedMultiSamplingCounts(PixelFormat format) const override;

private:
	// Internal functionality

	void RemoveSwapChain();
	void CreateSwapChain(DXGI_SWAP_CHAIN_DESC& desc);
	void EnsureFullscreen(bool fullscreen, IDXGIOutput* output);
	void UpdateRTV(uint32_t multiSamplingCount);

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