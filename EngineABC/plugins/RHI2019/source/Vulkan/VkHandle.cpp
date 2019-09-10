#include "LowLevel/Vulkan/VkHandle.h"

namespace ZHU
{
	template<> VkHandle<VkSwapchainKHR>::~VkHandle()
	{
		if (mNativeHandle != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(gpDevice->GetNativeHandle(), mNativeHandle, nullptr);
			mNativeHandle = VK_NULL_HANDLE;
		}
	}

	template<> VkHandle<VkCommandBuffer>::~VkHandle()
	{
		mNativeHandle = VK_NULL_HANDLE;
	}

	template<> VkHandle<VkCommandPool>::~VkHandle()
	{
		if (mNativeHandle != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(gpDevice->GetNativeHandle(), mNativeHandle, nullptr);
			mNativeHandle = VK_NULL_HANDLE;
		}
	}

	template<> VkHandle<VkQueue>::~VkHandle()
	{
		mNativeHandle = VK_NULL_HANDLE;
	}

	template<> VkHandle<VkSemaphore>::~VkHandle()
	{
		if (mNativeHandle != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(gpDevice->GetNativeHandle(), mNativeHandle, nullptr);
			mNativeHandle = VK_NULL_HANDLE;
		}
	}

	template<> VkHandle<VkSampler>::~VkHandle()
	{
		if (mNativeHandle != VK_NULL_HANDLE)
		{
			vkDestroySampler(gpDevice->GetNativeHandle(), mNativeHandle, nullptr);
			mNativeHandle = VK_NULL_HANDLE;
		}
	}

	template<> VkHandle<VkDescriptorSet>::~VkHandle()
	{
		mNativeHandle = VK_NULL_HANDLE;
	}

	template<> VkHandle<VkDescriptorSetLayout>::~VkHandle()
	{
		if (mNativeHandle != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(gpDevice->GetNativeHandle(), mNativeHandle, nullptr);
			mNativeHandle = VK_NULL_HANDLE;
		}
	}

	template<> VkHandle<VkPipeline>::~VkHandle()
	{
		if (mNativeHandle != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(gpDevice->GetNativeHandle(), mNativeHandle, nullptr);
			mNativeHandle = VK_NULL_HANDLE;
		}
	}

	template<> VkHandle<VkShaderModule>::~VkHandle()
	{
		if (mNativeHandle != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(gpDevice->GetNativeHandle(), mNativeHandle, nullptr);
			mNativeHandle = VK_NULL_HANDLE;
		}
	}

	template<> VkHandle<VkPipelineLayout>::~VkHandle()
	{
		if (mNativeHandle != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(gpDevice->GetNativeHandle(), mNativeHandle, nullptr);
			mNativeHandle = VK_NULL_HANDLE;
		}
	}

	template<> VkHandle<VkDescriptorPool>::~VkHandle()
	{
		if (mNativeHandle != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(gpDevice->GetNativeHandle(), mNativeHandle, nullptr);
			mNativeHandle = VK_NULL_HANDLE;
		}
	}

	template<> VkHandle<VkQueryPool>::~VkHandle()
	{
		if (mNativeHandle != VK_NULL_HANDLE)
		{
			vkDestroyQueryPool(gpDevice->GetNativeHandle(), mNativeHandle, nullptr);
			mNativeHandle = VK_NULL_HANDLE;
		}
	}

}