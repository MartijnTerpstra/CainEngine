#include "Precomp.h"
#include "VulkanRenderer.h"

using namespace ::CainEngine::Graphics;

unique_ptr<Implementation::IRenderer> Vulkan::CreateInstance()
{
	return make_unique<Vulkan::VulkanRenderer>();
}