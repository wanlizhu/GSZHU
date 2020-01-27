#pragma once

#include "VulkanRHI/IBufferVk.h"

namespace Wanlix
{
    class CBufferVk : public IBufferVk
    {
    public:

    protected:
        CBufferVk(IDevice* device,
                  const Desc& desc,
                  const char* data,
                  const String& name)
        {}

    protected:
        VkBuffer mVkBuffer = VK_NULL_HANDLE;
        VkDeviceMemory mVkMemory = VK_NULL_HANDLE;
    };
}