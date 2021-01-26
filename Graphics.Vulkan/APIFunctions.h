#pragma once

namespace CainEngine::Graphics::Vulkan {

#define GRAPHICS_DECLARE_INST_FUNCTION(x) PFN_##x x = null;
#define GRAPHICS_DECLARE_DEV_FUNCTION(x) PFN_##x x = null;

class APIFunctions
{
public:
	// Main functionality

	APIFunctions(const shared_ptr<VkInstance_T>& instance, VkDevice device, bool debuglayer);
	~APIFunctions();

public:
	// Member properties

	GRAPHICS_DECLARE_INST_FUNCTION(vkCreateDebugReportCallbackEXT);
	GRAPHICS_DECLARE_INST_FUNCTION(vkDestroyDebugReportCallbackEXT);

	GRAPHICS_DECLARE_DEV_FUNCTION(vkCreateSwapchainKHR);
	GRAPHICS_DECLARE_DEV_FUNCTION(vkDestroySwapchainKHR);
	GRAPHICS_DECLARE_DEV_FUNCTION(vkGetSwapchainImagesKHR);
	GRAPHICS_DECLARE_DEV_FUNCTION(vkAcquireNextImageKHR);
	GRAPHICS_DECLARE_DEV_FUNCTION(vkQueuePresentKHR);

	const VkInstance Instance;
	const VkDevice Device;

private:
	// Member variables

	VkDebugReportCallbackEXT m_debugCallback;
	const shared_ptr<VkInstance_T> m_instanceHandle;

}; // class APIFunctions

#undef GRAPHICS_DECLARE_INST_FUNCTION
#undef GRAPHICS_DECLARE_DEV_FUNCTION

};