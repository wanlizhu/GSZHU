#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/IDeviceContext.h"

namespace Wanlix
{
    class RHI_API IDeviceContextVk : public IDeviceContext
    {
        CLASSINFO(IDeviceContextVk)
        INHERIT_SHARED_FROM_THIS(IDeviceContext)
    public:
        virtual void TransitionImageLayout(ITexture* texture, VkImageLayout layout) = 0;
        virtual void BufferBarrier(IBuffer* buffer, VkAccessFlags accessFlags) = 0;
    };
}