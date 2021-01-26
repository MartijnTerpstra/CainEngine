#pragma once

#include "External/Graphics.Vulkan.h"

#include <vulkan/vulkan.h>

#include "APIFunctions.h"
#include "VkPtr.h"

#define CHECK_VKESULT(x) CheckVkResult(x, __FUNCTION__ "(): call failed: " #x)

inline void CheckVkResult(VkResult res, const char* errorMessage)
{
	if (res != VK_SUCCESS)
	{
		int resultCode = (long)res;
		Common::FatalError("%s VkResult: %d", errorMessage, resultCode);
	}
}