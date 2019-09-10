#include "LowLevel/Vulkan/VkResource.h"

namespace ZHU
{
	template<>
	VkResource<VkBuffer, VkImage>::~VkResource()
	{
		if (!gpDevice) 
			return; // This is here because of the black texture in VkResourceViews.cpp
		
		// All of our resources are allocated with memory,
		// except for the swap-chain backbuffers that we shouldn't release
		assert(mDeviceMemory || mType == VkResourceType::Image); 
		if (mDeviceMemory != VK_NULL_HANDLE)
		{
			switch (mType)
			{
			case VkResourceType::Image:
				vkDestroyImage(gpDevice->getApiHandle(), mImageHandle, nullptr);
				mImageHandle = VK_NULL_HANDLE;
				break;
			case VkResourceType::Buffer:
				vkDestroyBuffer(gpDevice->getApiHandle(), mBufferHandle, nullptr);
				mBufferHandle = VK_NULL_HANDLE;
				break;
			default:
				assert(false);
			}
			vkFreeMemory(gpDevice->GetNativeHandle(), mDeviceMemory, nullptr);
			mDeviceMemory = VK_NULL_HANDLE;
		}
	}

	template<>
	VkResource<VkBufferView, VkImageView>::~VkResource()
	{
		if (!gpDevice) 
			return; // This is here because of the black texture in VkResourceViews.cpp

		if (mImageHandle != VK_NULL_HANDLE || mBufferHandle != VK_NULL_HANDLE)
		{
			switch (mType)
			{
			case VkResourceType::Image:
				vkDestroyImageView(gpDevice->getApiHandle(), mImageHandle, nullptr);
				mImageHandle = VK_NULL_HANDLE;
				break;
			case VkResourceType::Buffer:
				vkDestroyBufferView(gpDevice->getApiHandle(), mBufferHandle, nullptr);
				mBufferHandle = VK_NULL_HANDLE;
				break;
			default:
				assert(false);
			}
		}
	}

}