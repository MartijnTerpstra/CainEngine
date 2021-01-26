#include "Precomp.h"

#include "APIFunctions.h"

using namespace ::CainEngine::Graphics;
using namespace ::CainEngine::Graphics::Vulkan;

#define GRAPHICS_INIT_INST_FUNCTION(x) x = (PFN_##x)vkGetInstanceProcAddr(Instance, #x); \
if(x == null) Common::FatalError("APIFunctions::Init(): extensions function not loaded: " #x)

#define GRAPHICS_INIT_DEV_FUNCTION(x) x = (PFN_##x)vkGetDeviceProcAddr(Device, #x); \
if(x == null) Common::FatalError("APIFunctions::Init(): extensions function not loaded: " #x)


VKAPI_ATTR VkBool32 VKAPI_CALL
OnVulkanError(VkFlags msgFlags, VkDebugReportObjectTypeEXT,
	uint64_t, size_t, int32_t msgCode,
	const char *pLayerPrefix, const char *pMsg, void *)
{
	COMMON_CALLSTACK_CALL;

	if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{

		// WTF Vulkan is completely buggy when it comes to image layouts

		if (strstr(pMsg, "You cannot transition the layout") == pMsg ||
			strstr(pMsg, "Images passed to present must be in layout") == pMsg)
			return false;

		Common::Error("Vulkan API ERROR: [%s] Code %d : %s", pLayerPrefix, msgCode,
			pMsg);

		exit(EXIT_FAILURE);

	}
	else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		if (strstr(pMsg, "vkGetFenceStatus called for fence") ||
			strstr(pMsg, "vkWaitForFences called for fence") ||
			strstr(pMsg, "submitted to VkResetFences in UNSIGNALED STATE") ||
			strstr(pMsg, "prior to any Draw Cmds. It is recommended you use RenderPass LOAD_OP_CLEAR on Attachments prior to any Draw.")
			)
			return false;
		Common::Warning("Vulkan API WARNING: [%s] Code %d : %s", pLayerPrefix, msgCode,
			pMsg);

		//exit(EXIT_FAILURE);
	}
	else
	{
		return false;
	}

	/*
	* false indicates that layer should not bail-out of an
	* API call that had validation failures. This may mean that the
	* app dies inside the driver due to invalid parameter(s).
	* That's what would happen without validation layers, so we'll
	* keep that behavior here.
	*/
	return false;
}


APIFunctions::APIFunctions(const shared_ptr<VkInstance_T>& instance, VkDevice device,
	bool debuglayer)
	: Instance(instance.get()),
	Device(device),
	m_debugCallback(0),
	m_instanceHandle(instance)
{
	if (debuglayer)
	{
		GRAPHICS_INIT_INST_FUNCTION(vkCreateDebugReportCallbackEXT);
		GRAPHICS_INIT_INST_FUNCTION(vkDestroyDebugReportCallbackEXT);

		VkDebugReportCallbackCreateInfoEXT debugInfo = { };

		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debugInfo.pNext = NULL;
		debugInfo.pfnCallback = OnVulkanError;
		debugInfo.pUserData = NULL;
		debugInfo.flags =
			VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;

		CheckVkResult(vkCreateDebugReportCallbackEXT(Instance, &debugInfo, null, &m_debugCallback),
			"Graphics::Internal::VulkanObjectFactory::VulkanObjectFactory(): call failed: vkCreateDebugReportCallbackEXT");
	}

	GRAPHICS_INIT_DEV_FUNCTION(vkCreateSwapchainKHR);
	GRAPHICS_INIT_DEV_FUNCTION(vkDestroySwapchainKHR);
	GRAPHICS_INIT_DEV_FUNCTION(vkGetSwapchainImagesKHR);
	GRAPHICS_INIT_DEV_FUNCTION(vkAcquireNextImageKHR);
	GRAPHICS_INIT_DEV_FUNCTION(vkQueuePresentKHR);
}

APIFunctions::~APIFunctions()
{
	vkDestroyDevice(Device, null);

	if (m_debugCallback)
		vkDestroyDebugReportCallbackEXT(Instance, m_debugCallback, null);
}