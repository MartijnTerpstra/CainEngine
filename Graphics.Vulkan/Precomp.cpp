#include "Precomp.h"
#include "VulkanRenderer.h"

using namespace ::CainEngine::Graphics;

unique_ptr<Implementation::IRenderer> Vulkan::createInstance()
{
	return make_unique<Vulkan::VulkanRenderer>();
}