#pragma once

#include "Wanlix/VulkanRHI/IBufferVk.h"

namespace Wanlix
{
    class CBufferVk : public IBufferVk
    {
        CLASSINFO(CBufferVk)
        INHERIT_SHARED_FROM_THIS(IBufferVk)
    public:
        virtual VkBuffer GetVkBuffer() const override;
        virtual void SetAccessFlags(VkAccessFlags flags) override;
        virtual VkAccessFlags GetAccessFlags() const override;
    };
}