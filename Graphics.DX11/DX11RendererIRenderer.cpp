#include "Precomp.h"

#include "InputLayoutResolver.h"
#include "DX11Texture.h"
#include "DX11Factory.h"
#include "DX11Renderer.h"

#include "EnumConverter.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX11;

uint32_t DX11Renderer::ID() const noexcept
{
	return Common::PackChars('d', 'x', '1', '1');
}

string DX11Renderer::ShortName() const noexcept
{
	return "DX11";
}

string DX11Renderer::Name() const noexcept
{
	return "DirectX 11";
}

void DX11Renderer::Init(flag<RendererInitFlags> initFlags)
{
	COMMON_CALLSTACK_CALL;

	auto createDeviceFunc = reinterpret_cast<decltype(D3D11CreateDevice)*>(GetProcAddress(m_d3d11, "D3D11CreateDevice"));

	D3D_FEATURE_LEVEL levels[2] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

	DWORD flags = 0;
	if (initFlags.is_enabled(RendererInitFlags::ApiDebug))
	{
		D3D_FEATURE_LEVEL selectedLvl;

		CHECK_HRESULT(createDeviceFunc(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, levels, 2, D3D11_SDK_VERSION, nullptr, &selectedLvl, nullptr));

		if (selectedLvl == D3D_FEATURE_LEVEL_11_1)
		{
			flags = D3D11_CREATE_DEVICE_DEBUG; // D3D11_CREATE_DEVICE_DEBUGGABLE;
		}
		else
		{
			flags = D3D11_CREATE_DEVICE_DEBUG;
		}
	}

	CHECK_HRESULT(createDeviceFunc(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levels, 2, D3D11_SDK_VERSION, mst::initialize(m_device), nullptr, mst::initialize(m_context)));

	com_ptr<IDXGIDevice> device;
	CHECK_HRESULT(m_device->QueryInterface<IDXGIDevice>(mst::initialize(device)));

	CHECK_HRESULT(device->GetAdapter(mst::initialize(m_adapter)));


	{
		D3D11_SAMPLER_DESC desc = {};

		desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.MaxAnisotropy = 1;

		com_ptr<ID3D11SamplerState> samplers[2];

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		CHECK_HRESULT(m_device->CreateSamplerState(&desc, mst::initialize(samplers[0])));
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		CHECK_HRESULT(m_device->CreateSamplerState(&desc, mst::initialize(samplers[1])));

		ID3D11SamplerState* ptrs[2] = { samplers[0].get(), samplers[1].get() };

		m_context->PSSetSamplers(1, 2, ptrs);
	}
}

void DX11Renderer::Exit()
{
	COMMON_CALLSTACK_CALL;

	Flush();

	m_inputLayoutResolver.Clear();

	RemoveSwapChain();
	m_mainWindow = nullptr;

	m_context->ClearState();
	m_context->Flush();

	m_factory.Clear();

	auto tempCopy = m_device;
	m_context.reset();
	if (m_device.reset() > 1) // just the copy
	{
		auto debug = tempCopy.as<ID3D11Debug>();
		auto queue = tempCopy.as<ID3D11InfoQueue>();

		if (debug != nullptr && queue != nullptr)
		{
			queue->ClearStoredMessages();

			CHECK_HRESULT(debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL));

			UINT64 messageCount = queue->GetNumStoredMessages();

			for (UINT64 messageIndex = 0; messageIndex < messageCount; ++messageIndex)
			{
				SIZE_T messageSize;
				CHECK_HRESULT(queue->GetMessageA(messageIndex, nullptr, &messageSize));

				D3D11_MESSAGE* message = (D3D11_MESSAGE*)malloc(messageSize);
				CHECK_HRESULT(queue->GetMessageA(messageIndex, message, &messageSize));

				Common::Error("%s", message->pDescription);

				free(message);
			}
		}

		Common::FatalError("Reference leak in DX11Renderer::Exit()");
	}
}

void DX11Renderer::Flush()
{
	m_context->Flush();
}

void DX11Renderer::SetMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow, const optional<SwapChainCreationSettings>& creationSettings)
{
	COMMON_CALLSTACK_CALL;

	if (m_mainWindow == mainWindow)
		return;

	m_mainWindow = mainWindow->As<Platform::Win32::IWin32Window>();

	if (!m_mainWindow)
	{ // unset main window
		return;
	}

	auto hwnd = m_mainWindow->GetHwnd();

	auto rect = m_mainWindow->GetClientRect();

	DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
	uint32_t refreshRate = 60;
	uint32_t preferredOutput = 0;
	uint32_t multiSamplingCount = 1;

	if (creationSettings)
	{
		format = EnumConverter::Convert(creationSettings->pixelFormat);
		refreshRate = creationSettings->refreshRate;
		preferredOutput = creationSettings->preferredOutput;
		creationSettings->multiSamplingCount;

		m_vSync = creationSettings->vSync;
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
	swapChainDesc.BufferCount = 2; // double buffering
	swapChainDesc.BufferDesc.Width = rect.GetWidth();
	swapChainDesc.BufferDesc.Height = rect.GetHeight();
	swapChainDesc.BufferDesc.Format = format;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = refreshRate;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;

	com_ptr<IDXGIOutput> output;
	CHECK_HRESULT(m_adapter->EnumOutputs(preferredOutput, mst::initialize(output)));

	DXGI_MODE_DESC closestMatch;
	CHECK_HRESULT(output->FindClosestMatchingMode(&swapChainDesc.BufferDesc, &closestMatch, m_device.get()));

	swapChainDesc.BufferDesc.RefreshRate = closestMatch.RefreshRate;

	CreateSwapChain(swapChainDesc);
}

void CainEngine::Graphics::DX11::DX11Renderer::HandleWindowResize()
{
	if (!m_mainWindow)
		return;

	Flush();

	m_backbuffer.reset();
	m_backBufferRTV.reset();

	auto rect = m_mainWindow->GetClientRect();

	m_swapChain->ResizeBuffers(2, (UINT)rect.GetWidth(), (UINT)rect.GetHeight(), DXGI_FORMAT_B8G8R8A8_UNORM, 0);

	CHECK_HRESULT(m_swapChain->GetBuffer(0, MST_IID_PPV_ARGS(m_backbuffer)));

	UpdateRTV(MultiSamplingCount());
}

bool DX11Renderer::HasFeature(RendererFeature feature) const
{
	COMMON_CALLSTACK_CALL;

	switch (feature)
	{
	case CainEngine::Graphics::RendererFeature::AsyncShaderLoading:
		return true;
	case CainEngine::Graphics::RendererFeature::MultiThreadedCommandLists:
		return false;
	default:
		Common::FatalError("Unimplemented feature");
	}
}

uint2 CainEngine::Graphics::DX11::DX11Renderer::GetBackBufferSize() const noexcept
{
	D3D11_TEXTURE2D_DESC desc;
	m_backbuffer->GetDesc(&desc);

	return { desc.Width, desc.Height };
}

API::IDisplaySettings& DX11Renderer::DisplaySettings() noexcept
{
	return *this;
}

API::IFactory* DX11Renderer::GetFactory() noexcept
{
	return &m_factory;
}