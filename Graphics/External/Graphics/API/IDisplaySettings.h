#pragma once

namespace CainEngine::Graphics::API {

class IDisplaySettings
{
	COMMON_DECLARE_INTERFACE(IDisplaySettings);
public:
	// Main functionality

	virtual FullScreenState FullScreen() const = 0;

	virtual void SetFullScreen(bool fullScreen, uint32_t outputIndex) = 0;

	virtual uint32_t MultiSamplingCount() const = 0;

	virtual void SetMultiSamplingCount(uint32_t count) = 0;

	virtual bool VSync() const = 0;

	virtual void SetVSync(bool vSync) = 0;

	virtual PixelFormat Format() const = 0;

	virtual void SetFormat(PixelFormat format) = 0;

	virtual SwapChainDisplayMode DisplayMode() const = 0;

	virtual void SetDisplayMode(const SwapChainDisplayMode& mode) = 0;

	virtual vector<OutputDisplay> SupportedOutputs() const = 0;

	virtual vector<PixelFormat> SupportedPixelFormats(uint32_t outputIndex) const = 0;

	virtual vector<SwapChainDisplayMode> SupportedDisplayModes(uint32_t outputIndex, PixelFormat format) const = 0;

	virtual vector<uint32_t> SupportedMultiSamplingCounts(PixelFormat format) const = 0;
};

inline IDisplaySettings::~IDisplaySettings() = default;

}