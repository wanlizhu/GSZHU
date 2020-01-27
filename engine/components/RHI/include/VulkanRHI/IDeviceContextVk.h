#pragma once

#include "RHI/IDeviceContext.h"
#include "VulkanRHI/VkUtilities.h"
#include "VulkanRHI/ITextureVk.h"
#include "VulkanRHI/IBufferVk.h"

namespace Wanlix
{
    class IDeviceContextVk : public IDeviceContext
    {
    public:
        virtual void TransitionImageLayout(ITextureVk* texture, VkImageLayout newLayout) = 0;
        virtual void BufferMemoryBarrier(IBufferVk* buffer, VkAccessFlags newFlags) = 0;
    };
}