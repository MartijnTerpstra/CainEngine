#include "Precomp.h"

#include "VulkanRenderer.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::Vulkan;

namespace {

[[noreturn]] void notImplemented(const char* function)
{
	Common::fatalError("Graphics::Vulkan::VulkanRenderer::%s(): not implemented", function);
}

} // namespace

VulkanRenderer::VulkanRenderer()
{ }

VulkanRenderer::~VulkanRenderer()
{ }

uint32_t VulkanRenderer::id() const noexcept
{
	return Common::packChars('v', 'l', 'k', 'n');
}

std::string VulkanRenderer::shortName() const noexcept
{
	return "Vulkan";
}

std::string VulkanRenderer::name() const noexcept
{
	return "Vulkan";
}

void VulkanRenderer::init(flag<RendererInitFlags> initFlags)
{
	notImplemented("init");
}

void VulkanRenderer::exit()
{
	notImplemented("exit");
}

void VulkanRenderer::renderFrame(std::function<void(API::IRenderContext*)> onRender)
{
	notImplemented("renderFrame");
}

void VulkanRenderer::flush()
{
	notImplemented("flush");
}

void VulkanRenderer::setMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow, const std::optional<SwapChainCreationSettings>& creationSettings)
{
	notImplemented("setMainWindow");
}

void VulkanRenderer::handleWindowResize()
{
	notImplemented("handleWindowResize");
}

bool VulkanRenderer::hasFeature(RendererFeature feature) const
{
	return false;
}

uint2 VulkanRenderer::getBackBufferSize() const noexcept
{
	return uint2(0, 0);
}

API::IDisplaySettings& VulkanRenderer::displaySettings() noexcept
{
	return *this;
}

API::IFactory* VulkanRenderer::getFactory() noexcept
{
	notImplemented("getFactory");
}

FullScreenState VulkanRenderer::fullScreen() const
{
	notImplemented("fullScreen");
}

void VulkanRenderer::setFullScreen(bool fullScreen, uint32_t outputIndex)
{
	notImplemented("setFullScreen");
}

uint32_t VulkanRenderer::multiSamplingCount() const
{
	notImplemented("multiSamplingCount");
}

void VulkanRenderer::setMultiSamplingCount(uint32_t count)
{
	notImplemented("setMultiSamplingCount");
}

bool VulkanRenderer::vSync() const
{
	notImplemented("vSync");
}

void VulkanRenderer::setVSync(bool vSync)
{
	notImplemented("setVSync");
}

PixelFormat VulkanRenderer::format() const
{
	notImplemented("format");
}

void VulkanRenderer::setFormat(PixelFormat format)
{
	notImplemented("setFormat");
}

SwapChainDisplayMode VulkanRenderer::displayMode() const
{
	notImplemented("displayMode");
}

void VulkanRenderer::setDisplayMode(const SwapChainDisplayMode& mode)
{
	notImplemented("setDisplayMode");
}

std::vector<OutputDisplay> VulkanRenderer::supportedOutputs() const
{
	notImplemented("supportedOutputs");
}

std::vector<PixelFormat> VulkanRenderer::supportedPixelFormats(uint32_t outputIndex) const
{
	notImplemented("supportedPixelFormats");
}

std::vector<SwapChainDisplayMode> VulkanRenderer::supportedDisplayModes(uint32_t outputIndex, PixelFormat format) const
{
	notImplemented("supportedDisplayModes");
}

std::vector<uint32_t> VulkanRenderer::supportedMultiSamplingCounts(PixelFormat format) const
{
	notImplemented("supportedMultiSamplingCounts");
}
