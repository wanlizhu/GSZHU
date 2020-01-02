#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/IBuffer.h"

namespace Wanlix
{
    class RHI_API IBufferVk : public IBuffer
    {
        CLASSINFO(IBufferVk)
        INHERIT_SHARED_FROM_THIS(IBuffer)
    public:
        virtual VkBuffer GetVkBuffer() const = 0;
        virtual void SetAccessFlags(VkAccessFlags flags) = 0;
        virtual VkAccessFlags GetAccessFlags() const = 0;
    };
}