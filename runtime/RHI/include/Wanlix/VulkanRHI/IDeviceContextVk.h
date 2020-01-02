#pragma once

#include "Common.h"
#include "Wanlix/RHI/IDeviceContext.h"

namespace Wanlix
{
    class IDeviceContextVk : public inherit_shared_from_this<IDeviceContext, IDeviceContextVk>
    {
    public:
        virtual void TransitionImageLayout(ITexture* texture, VkImageLayout layout) = 0;
        virtual void BufferBarrier(IBuffer* buffer, VkAccessFlags accessFlags) = 0;
    };
}