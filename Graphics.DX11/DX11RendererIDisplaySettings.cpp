#include "Precomp.h"

#include "InputLayoutResolver.h"
#include "DX11Texture.h"
#include "DX11Factory.h"
#include "DX11Renderer.h"

#include "EnumConverter.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::DX11;

FullScreenState DX11Renderer::FullScreen() const
{
	COMMON_CALLSTACK_CALL;

	if (!m_swapChain)
		Common::FatalError("No main window set");

	BOOL fullscreen;
	com_ptr<IDXGIOutput> output;
	CHECK_HRESULT(m_swapChain->GetFullscreenState(&fullscreen, mst::initialize(output)));

	if (output)
	{
		DXGI_OUTPUT_DESC desc;
		CHECK_HRESULT(output->GetDesc(&desc));

		uint32_t index = 0;
		com_ptr<IDXGIOutput> o;
		while (m_adapter->EnumOutputs((UINT)index, mst::initialize(o)) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_OUTPUT_DESC od;
			CHECK_HRESULT(o->GetDesc(&od));

			if (desc.Monitor == od.Monitor)
			{
				return FullScreenState(fullscreen != FALSE, index);
			}

			++index;
		}
	}

	return FullScreenState(fullscreen != FALSE, 0);
}

void DX11Renderer::SetFullScreen(bool fullScreen, uint32_t outputIndex)
{
	COMMON_CALLSTACK_CALL;

	if (!m_swapChain)
		Common::FatalError("No main window set");

	if (fullScreen)
	{
		com_ptr<IDXGIOutput> output;
		CHECK_HRESULT(m_adapter->EnumOutputs((UINT)outputIndex, mst::initialize(output)));

		CHECK_HRESULT(m_swapChain->SetFullscreenState(TRUE, output.get()));
	}
	else
	{
		CHECK_HRESULT(m_swapChain->SetFullscreenState(FALSE, nullptr));
	}
}

uint32_t DX11Renderer::MultiSamplingCount() const
{
	COMMON_CALLSTACK_CALL;

	if (!m_swapChain)
		Common::FatalError("No main window set");

	if (!m_msBackBuffer)
		return 1;

	D3D11_TEXTURE2D_DESC desc;
	m_msBackBuffer->GetDesc(&desc);

	return desc.SampleDesc.Count;
}

void DX11Renderer::SetMultiSamplingCount(uint32_t count)
{
	COMMON_CALLSTACK_CALL;

	if (!m_swapChain)
		Common::FatalError("No main window set");

	if (count == MultiSamplingCount())
		return; // no changes

	m_msBackBuffer.reset();
	m_backBufferRTV.reset();

	DXGI_SWAP_CHAIN_DESC swDesc;
	CHECK_HRESULT(m_swapChain->GetDesc(&swDesc));

	UINT qualityLevels;
	CHECK_HRESULT(m_device->CheckMultisampleQualityLevels(swDesc.BufferDesc.Format, count, &qualityLevels));

	if (qualityLevels == 0)
	{
		Common::Error("invalid argument: count (%u), sampling count is not supported, see GetSupportedMultiSamplingCounts()", count);
		return; // dont commit change
	}

	UpdateRTV(count);
}

bool DX11Renderer::VSync() const
{
	return m_vSync;
}

void DX11Renderer::SetVSync(bool vSync)
{
	m_vSync = vSync;
}

PixelFormat DX11Renderer::Format() const
{
	COMMON_CALLSTACK_CALL;

	if (!m_swapChain)
		Common::FatalError("No main window set");

	DXGI_SWAP_CHAIN_DESC desc;
	CHECK_HRESULT(m_swapChain->GetDesc(&desc));

	return EnumConverter::Convert(desc.BufferDesc.Format);
}

void DX11Renderer::SetFormat(PixelFormat format)
{
	COMMON_CALLSTACK_CALL;

	if (!m_swapChain)
		Common::FatalError("No main window set");

	Common::FatalError("unimplemented");
}

SwapChainDisplayMode DX11Renderer::DisplayMode() const
{
	COMMON_CALLSTACK_CALL;

	if (!m_swapChain)
		Common::FatalError("No main window set");

	DXGI_SWAP_CHAIN_DESC desc;
	CHECK_HRESULT(m_swapChain->GetDesc(&desc));

	uint32_t refreshRate = (uint32_t)(desc.BufferDesc.RefreshRate.Numerator / (float)desc.BufferDesc.RefreshRate.Denominator + 0.5f);

	return SwapChainDisplayMode(desc.BufferDesc.Width, desc.BufferDesc.Height, refreshRate);
}

void DX11Renderer::SetDisplayMode(const SwapChainDisplayMode& mode)
{
	COMMON_CALLSTACK_CALL;

	if (!m_swapChain)
		Common::FatalError("No main window set");

	Common::FatalError("unimplemented");
}

vector<OutputDisplay> DX11Renderer::SupportedOutputs() const
{
	vector<OutputDisplay> outputs;

	uint32_t index = 0;
	com_ptr<IDXGIOutput> o;
	while (m_adapter->EnumOutputs((UINT)index, mst::initialize(o)) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC od;
		CHECK_HRESULT(o->GetDesc(&od));

		auto charCount = WideCharToMultiByte(CP_UTF8, 0, od.DeviceName, (int)wcslen(od.DeviceName), nullptr, 0, nullptr, nullptr);


		string result;
		if(charCount > 0)
		{
			result.resize((size_t)charCount, ' ');
			WideCharToMultiByte(CP_UTF8, 0, od.DeviceName, (int)wcslen(od.DeviceName), result.data(), charCount, nullptr, nullptr);
		}

		outputs.emplace_back(OutputDisplay(index, std::move(result)));

		++index;
	}

	return outputs;
}

vector<PixelFormat> DX11Renderer::SupportedPixelFormats(uint32_t output) const
{
	vector<PixelFormat> retval;

	UINT formatSupport;
	CHECK_HRESULT(m_device->CheckFormatSupport(DXGI_FORMAT_R8G8B8A8_UNORM, &formatSupport));

	if (formatSupport & D3D11_FORMAT_SUPPORT_DISPLAY)
		retval.push_back(PixelFormat::UnormRGBA8);

	CHECK_HRESULT(m_device->CheckFormatSupport(DXGI_FORMAT_B8G8R8A8_UNORM, &formatSupport));
	if (formatSupport & D3D11_FORMAT_SUPPORT_DISPLAY)
		retval.push_back(PixelFormat::UnormBGRA8);

	CHECK_HRESULT(m_device->CheckFormatSupport(DXGI_FORMAT_R16G16B16A16_FLOAT, &formatSupport));
	if (formatSupport & D3D11_FORMAT_SUPPORT_DISPLAY)
		retval.push_back(PixelFormat::FloatRGBA16);

	return retval;
}

vector<SwapChainDisplayMode> DX11Renderer::SupportedDisplayModes(uint32_t outputIndex, PixelFormat format) const
{
	com_ptr<IDXGIOutput> output;
	CHECK_HRESULT(m_adapter->EnumOutputs((UINT)outputIndex, mst::initialize(output)));

	auto dxformat = EnumConverter::Convert(format);

	UINT numModes;
	CHECK_HRESULT(output->GetDisplayModeList(dxformat, 0, &numModes, nullptr));

	vector<DXGI_MODE_DESC> descs(numModes);
	CHECK_HRESULT(output->GetDisplayModeList(dxformat, 0, &numModes, descs.data()));

	vector<SwapChainDisplayMode> displayModes;
	displayModes.reserve(numModes);

	for (auto& desc : descs)
	{
		uint32_t refreshRate = (uint32_t)(desc.RefreshRate.Numerator / (float)desc.RefreshRate.Denominator + 0.5f);

		displayModes.emplace_back(desc.Width, desc.Height, refreshRate);
	}

	return displayModes;
}

vector<uint32_t> DX11Renderer::SupportedMultiSamplingCounts(PixelFormat format) const
{
	vector<uint32_t> modes;

	auto dxformat = EnumConverter::Convert(format);

	for (UINT i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1)
	{
		UINT qualityLevels;
		CHECK_HRESULT(m_device->CheckMultisampleQualityLevels(dxformat, i, &qualityLevels));

		if (qualityLevels > 0)
			modes.push_back(i);
	}

	return modes;
}

void DX11Renderer::EnsureFullscreen(bool fullscreen, IDXGIOutput* output)
{
	Common::FatalError("unimplemented");
}