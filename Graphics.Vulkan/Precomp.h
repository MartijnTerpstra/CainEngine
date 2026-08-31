#pragma once

#include "External/Graphics.Vulkan.h"

#include <vulkan/vulkan.h>

#include "APIFunctions.h"
#include "VkPtr.h"

#define CHECK_VKESULT(x) checkVkResult(x, __FUNCTION__ "(): call failed: " #x)

inline void checkVkResult(VkResult res, const char* errorMessage)
{
	if (res != VK_SUCCESS)
	{
		int resultCode = (long)res;
		CainEngine::Common::fatalError("%s VkResult: %d", errorMessage, resultCode);
	}
}
