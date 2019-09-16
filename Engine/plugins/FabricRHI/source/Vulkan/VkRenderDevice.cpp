#include "LowLevel/Vulkan/VkRenderDevice.h"

namespace ZHU
{
	VkRenderDevice::~VkRenderDevice()
	{
		if (mInstance != VK_NULL_HANDLE &&
			mLogicalDevice != VK_NULL_HANDLE &&
			mSurface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
			vkDestroyDevice(mLogicalDevice, nullptr);
			vkDestroyInstance(mInstance, nullptr);

			mInstance = VK_NULL_HANDLE;
			mLogicalDevice = VK_NULL_HANDLE;
			mPhysicalDevice = VK_NULL_HANDLE;
			mSurface = VK_NULL_HANDLE;
		}
	}
}