#pragma once

namespace CainEngine::Graphics::API {

class IDisplaySettings
{
	COMMON_DECLARE_INTERFACE(IDisplaySettings);
public:
	// Main functionality

	virtual FullScreenState fullScreen() const = 0;

	virtual void setFullScreen(bool fullScreen, uint32_t outputIndex) = 0;

	virtual uint32_t multiSamplingCount() const = 0;

	virtual void setMultiSamplingCount(uint32_t count) = 0;

	virtual bool vSync() const = 0;

	virtual void setVSync(bool vSync) = 0;

	virtual PixelFormat format() const = 0;

	virtual void setFormat(PixelFormat format) = 0;

	virtual SwapChainDisplayMode displayMode() const = 0;

	virtual void setDisplayMode(const SwapChainDisplayMode& mode) = 0;

	virtual std::vector<OutputDisplay> supportedOutputs() const = 0;

	virtual std::vector<PixelFormat> supportedPixelFormats(uint32_t outputIndex) const = 0;

	virtual std::vector<SwapChainDisplayMode> supportedDisplayModes(uint32_t outputIndex, PixelFormat format) const = 0;

	virtual std::vector<uint32_t> supportedMultiSamplingCounts(PixelFormat format) const = 0;
};

inline IDisplaySettings::~IDisplaySettings() = default;

}