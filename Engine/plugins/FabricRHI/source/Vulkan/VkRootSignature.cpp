#include "LowLevel/Vulkan/VkRootSignature.h"

namespace ZHU
{
	VkRootSignature::~VkRootSignature()
	{
		if (mNativeHandle != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(gpDevice->GetNativeHandle(), mNativeHandle, nullptr);
			for (auto& set : mDescriptorSets)
			{
				vkDestroyDescriptorSetLayout(gpDevice->GetNativeHandle(), set, nullptr);
			}

			mNativeHandle = NULL;
			mDescriptorSets.clear();
		}
	}
}