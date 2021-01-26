#pragma once

namespace CainEngine {
namespace Graphics {
namespace DX12 {

class DX12Renderer final : public Implementation::IRenderer
{
public:
	// ctor & dtor

	DX12Renderer();
	~DX12Renderer();

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

	uint m_renderIndex = 0;

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