#pragma once

#include "RHI/IDeviceContext.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class IBufferVk;
    class ITextureVk;

    class IDeviceContextVk : public IDeviceContext
    {
    public:
        virtual void TransitionImageLayout(ITextureVk* texture, VkImageLayout newLayout) = 0;
        virtual void BufferMemoryBarrier(IBufferVk* buffer, VkAccessFlags newFlags) = 0;
    };
}