#include "Precomp.h"
#include "VulkanRenderer.h"

using namespace ::CainEngine::Graphics;

std::unique_ptr<API::IRenderer> Vulkan::createInstance()
{
	return std::make_unique<Vulkan::VulkanRenderer>();
}
