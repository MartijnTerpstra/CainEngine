#include "Precomp.h"

#include "Event.h"
#include "DX12Renderer.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX12;

namespace {

[[noreturn]] void notImplemented(const char* function)
{
	Common::fatalError("Graphics::DX12::DX12Renderer::%s(): not implemented", function);
}

} // namespace

DX12Renderer::DX12Renderer()
{ }

DX12Renderer::~DX12Renderer()
{ }

uint32_t DX12Renderer::id() const noexcept
{
	return Common::packChars('d', 'x', '1', '2');
}

std::string DX12Renderer::shortName() const noexcept
{
	return "DX12";
}

std::string DX12Renderer::name() const noexcept
{
	return "DirectX 12";
}

void DX12Renderer::init(flag<RendererInitFlags> initFlags)
{
	COMMON_CALLSTACK_CALL;

	UINT flags = 0;

#if !defined(C)

	flags |= DXGI_CREATE_FACTORY_DEBUG;

	com_ptr<ID3D12Debug> debugController;
	if(SUCCEEDED(D3D12GetDebugInterface(MST_IID_PPV_ARGS(debugController))))
	{
		debugController->EnableDebugLayer();
	}

#endif

	CHECK_HRESULT(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, MST_IID_PPV_ARGS(m_factory)));

	CHECK_HRESULT(m_factory->EnumAdapterByGpuPreference(
		0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, MST_IID_PPV_ARGS(m_adapter)));

	CHECK_HRESULT(
		D3D12CreateDevice(m_adapter.get(), D3D_FEATURE_LEVEL_11_0, MST_IID_PPV_ARGS(m_device)));

	D3D_FEATURE_LEVEL lvls[]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_12_1,
	};

	D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelInfo;
	featureLevelInfo.pFeatureLevelsRequested = lvls;
	featureLevelInfo.NumFeatureLevels = (UINT)std::size(lvls);

	CHECK_HRESULT(m_device->CheckFeatureSupport(
		D3D12_FEATURE_FEATURE_LEVELS, &featureLevelInfo, sizeof(featureLevelInfo)));

	m_featureLvl = featureLevelInfo.MaxSupportedFeatureLevel;

	m_device.reset();

	CHECK_HRESULT(D3D12CreateDevice(m_adapter.get(), m_featureLvl, MST_IID_PPV_ARGS(m_device)));

	D3D12_COMMAND_QUEUE_DESC desc = {};

	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

	CHECK_HRESULT(m_device->CreateCommandQueue(&desc, MST_IID_PPV_ARGS(m_queue)));

	m_device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, MST_IID_PPV_ARGS(m_allocators[0]));
	m_device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, MST_IID_PPV_ARGS(m_allocators[1]));

	for(auto& evt : m_commandQueueCompletedEvents)
	{
		evt.init(m_device.get());
	}

	m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_allocators[0].get(), nullptr,
		MST_IID_PPV_ARGS(m_commandLists[0]));
	m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_allocators[1].get(), nullptr,
		MST_IID_PPV_ARGS(m_commandLists[1]));
}

void DX12Renderer::exit()
{ }

void DX12Renderer::renderFrame(std::function<void(API::IRenderContext*)> onRender)
{
	COMMON_CALLSTACK_CALL;

	if(m_swapChain == nullptr)
		return;

	uint32_t previousRenderIndex = (m_renderIndex + 1) & 1;

	uint32_t currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

	m_commandQueueCompletedEvents[m_renderIndex].waitTillCompletion();

	CHECK_HRESULT(m_allocators[m_renderIndex]->Reset());

	auto& renderQueue = m_commandLists[m_renderIndex];

	CHECK_HRESULT(renderQueue->Reset(m_allocators[m_renderIndex].get(), nullptr));

	auto trans = CD3DX12_RESOURCE_BARRIER::Transition(m_backbuffers[currentBackBufferIndex].get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	renderQueue->ResourceBarrier(1, &trans);

	// NOTE: onRender() isn't wired up yet - there's no API::IRenderContext
	// implementation backing this command list (see DX11's DX11RenderContext
	// for the shape one would need). This is where it would be invoked, in
	// between the two resource barrier transitions.

	trans = CD3DX12_RESOURCE_BARRIER::Transition(m_backbuffers[currentBackBufferIndex].get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	renderQueue->ResourceBarrier(1, &trans);

	renderQueue->Close();

	ID3D12CommandList* ptr = renderQueue.get();

	m_queue->ExecuteCommandLists(1, &ptr);

	m_commandQueueCompletedEvents->signalFence(m_queue.get());

	m_swapChain->Present(1, 0);

	m_renderIndex = (m_renderIndex + 1) & 1;
}

void DX12Renderer::flush()
{
	notImplemented("flush");
}

void DX12Renderer::setMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow, const std::optional<SwapChainCreationSettings>& creationSettings)
{
	COMMON_CALLSTACK_CALL;

	auto window = mainWindow->as<Platform::Win32::IWin32Window>();

	auto hwnd = window->getHwnd();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2; // double buffering
	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	com_ptr<IDXGISwapChain1> swapChain;
	CHECK_HRESULT(m_factory->CreateSwapChainForHwnd(
		m_device.get(), hwnd, &swapChainDesc, nullptr, nullptr, mst::initialize(swapChain)));

	m_swapChain = swapChain.as<IDXGISwapChain3>();

	CHECK_HRESULT(m_swapChain->GetBuffer(0, MST_IID_PPV_ARGS(m_backbuffers[0])));
	CHECK_HRESULT(m_swapChain->GetBuffer(1, MST_IID_PPV_ARGS(m_backbuffers[1])));
}

void DX12Renderer::handleWindowResize()
{
	notImplemented("handleWindowResize");
}

bool DX12Renderer::hasFeature(RendererFeature feature) const
{
	return false;
}

uint2 DX12Renderer::getBackBufferSize() const noexcept
{
	return uint2(0, 0);
}

API::IDisplaySettings& DX12Renderer::displaySettings() noexcept
{
	return *this;
}

API::IFactory* DX12Renderer::getFactory() noexcept
{
	notImplemented("getFactory");
}

FullScreenState DX12Renderer::fullScreen() const
{
	notImplemented("fullScreen");
}

void DX12Renderer::setFullScreen(bool fullScreen, uint32_t outputIndex)
{
	notImplemented("setFullScreen");
}

uint32_t DX12Renderer::multiSamplingCount() const
{
	notImplemented("multiSamplingCount");
}

void DX12Renderer::setMultiSamplingCount(uint32_t count)
{
	notImplemented("setMultiSamplingCount");
}

bool DX12Renderer::vSync() const
{
	notImplemented("vSync");
}

void DX12Renderer::setVSync(bool vSync)
{
	notImplemented("setVSync");
}

PixelFormat DX12Renderer::format() const
{
	notImplemented("format");
}

void DX12Renderer::setFormat(PixelFormat format)
{
	notImplemented("setFormat");
}

SwapChainDisplayMode DX12Renderer::displayMode() const
{
	notImplemented("displayMode");
}

void DX12Renderer::setDisplayMode(const SwapChainDisplayMode& mode)
{
	notImplemented("setDisplayMode");
}

std::vector<OutputDisplay> DX12Renderer::supportedOutputs() const
{
	notImplemented("supportedOutputs");
}

std::vector<PixelFormat> DX12Renderer::supportedPixelFormats(uint32_t outputIndex) const
{
	notImplemented("supportedPixelFormats");
}

std::vector<SwapChainDisplayMode> DX12Renderer::supportedDisplayModes(uint32_t outputIndex, PixelFormat format) const
{
	notImplemented("supportedDisplayModes");
}

std::vector<uint32_t> DX12Renderer::supportedMultiSamplingCounts(PixelFormat format) const
{
	notImplemented("supportedMultiSamplingCounts");
}
