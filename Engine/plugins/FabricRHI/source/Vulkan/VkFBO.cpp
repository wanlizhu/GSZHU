#include "LowLevel/Vulkan/VkFBO.h"

namespace ZHU
{
	VkFBO::~VkFBO()
	{
		if (mVkRenderPass != VK_NULL_HANDLE && mVkFramebuffer != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(gpDevice->GetNativeHandle(), mVkRenderPass, nullptr);
			vkDestroyFramebuffer(gpDevice->GetNativeHandle(), mVkFramebuffer, nullptr);

			mVkFramebuffer = VK_NULL_HANDLE;
			mVkRenderPass = VK_NULL_HANDLE;
		}
	}
}