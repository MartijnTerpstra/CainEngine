#include "Precomp.h"

#include "Event.h"
#include "DX12Renderer.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX12;

DX12Renderer::DX12Renderer()
{
}

DX12Renderer::~DX12Renderer()
{
}

void DX12Renderer::Init(flag<RendererInitFlags> initFlags)
{
	COMMON_CALLSTACK_CALL;

	UINT flags = 0;

#if !defined(C)

	flags |= DXGI_CREATE_FACTORY_DEBUG;

	com_ptr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(MST_IID_PPV_ARGS(debugController))))
	{
		debugController->EnableDebugLayer();
	}

#endif

	CHECK_HRESULT(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, MST_IID_PPV_ARGS(m_factory)));

	CHECK_HRESULT(m_factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, MST_IID_PPV_ARGS(m_adapter)));

	CHECK_HRESULT(D3D12CreateDevice(m_adapter.get(), D3D_FEATURE_LEVEL_11_0, MST_IID_PPV_ARGS(m_device)));

	D3D_FEATURE_LEVEL lvls[]
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_12_1,
	};

	D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelInfo;
	featureLevelInfo.pFeatureLevelsRequested = lvls;
	featureLevelInfo.NumFeatureLevels = (UINT)extentof(lvls);

	CHECK_HRESULT(m_device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelInfo, sizeof(featureLevelInfo)));

	m_featureLvl = featureLevelInfo.MaxSupportedFeatureLevel;

	m_device.reset();

	CHECK_HRESULT(D3D12CreateDevice(m_adapter.get(), m_featureLvl, MST_IID_PPV_ARGS(m_device)));

	D3D12_COMMAND_QUEUE_DESC desc = {};

	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

	CHECK_HRESULT(m_device->CreateCommandQueue(&desc, MST_IID_PPV_ARGS(m_queue)));

	m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, MST_IID_PPV_ARGS(m_allocators[0]));
	m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, MST_IID_PPV_ARGS(m_allocators[1]));

	for (auto& evt : m_commandQueueCompletedEvents)
	{
		evt.Init(m_device.get());
	}

	m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_allocators[0].get(), null, MST_IID_PPV_ARGS(m_commandLists[0]));
	m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_allocators[1].get(), null, MST_IID_PPV_ARGS(m_commandLists[1]));
}

void DX12Renderer::RenderFrame()
{
	COMMON_CALLSTACK_CALL;

	if (m_swapChain == null)
		return;

	uint previousRenderIndex = (m_renderIndex + 1) & 1;

	uint currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

	m_commandQueueCompletedEvents[m_renderIndex].WaitTillCompletion();

	CHECK_HRESULT(m_allocators[m_renderIndex]->Reset());

	auto& renderQueue = m_commandLists[m_renderIndex];

	CHECK_HRESULT(renderQueue->Reset(m_allocators[m_renderIndex].get(), null));

	auto trans = CD3DX12_RESOURCE_BARRIER::Transition(m_backbuffers[currentBackBufferIndex].get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	renderQueue->ResourceBarrier(1, &trans);
	/*
	renderQueue->ClearDepthStencilView();
	renderQueue->ClearRenderTargetView({ float4(0,0,0,0) }, 1, none);
	//renderQueue->SetResourceGroup(m_group);

	renderQueue->SetViewports({ Viewport((float)size.x, (float)size.y) });

	//renderQueue->SetInputBuffers({ }, m_indexBuffer);

	//renderQueue->Draw(3);

	for (auto& mesh : m_meshes)
	{
		mesh->Render(renderQueue.get());
	}
	*/

	trans = CD3DX12_RESOURCE_BARRIER::Transition(m_backbuffers[currentBackBufferIndex].get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	renderQueue->ResourceBarrier(1, &trans);

	renderQueue->Close();

	ID3D12CommandList* ptr = renderQueue.get();

	m_queue->ExecuteCommandLists(1, &ptr);

	m_commandQueueCompletedEvents->SignalFence(m_queue.get());

	//m_gpuStream->Flush();

	m_swapChain->Present(1, 0);

	m_renderIndex = (m_renderIndex + 1) & 1;
}

void DX12Renderer::Exit()
{
}

void DX12Renderer::SetMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow)
{
	COMMON_CALLSTACK_CALL;

	auto window = mainWindow->As<Platform::Win32::IWin32Window>();

	auto hwnd = window->GetHwnd();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { };
	swapChainDesc.BufferCount = 2; // double buffering
	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	com_ptr<IDXGISwapChain1> swapChain;
	CHECK_HRESULT(m_factory->CreateSwapChainForHwnd(m_device.get(), hwnd, &swapChainDesc, null, null, mst::initialize(swapChain)));

	m_swapChain = swapChain.as<IDXGISwapChain3>();

	CHECK_HRESULT(m_swapChain->GetBuffer(0, MST_IID_PPV_ARGS(m_backbuffers[0])));
	CHECK_HRESULT(m_swapChain->GetBuffer(1, MST_IID_PPV_ARGS(m_backbuffers[1])));
}