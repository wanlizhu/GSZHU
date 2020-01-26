#pragma once

#include "RHI/IDeviceContext.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class IDeviceContextVk : public IDeviceContext
    {
    public:
        virtual void TransitionImageLayout(ITexture* texture, VkImageLayout newLayout) = 0;
        virtual void BufferMemoryBarrier(IBuffer* buffer, VkAccessFlags newFlags) = 0;
    };
}