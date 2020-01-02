#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/IBufferView.h"

namespace Wanlix
{
    class RHI_API IBufferViewVk : public IBufferView
    {
        CLASSINFO(IBufferViewVk)
        INHERIT_SHARED_FROM_THIS(IBufferView)
    public:
        virtual VkBufferView GetVkBufferView() const = 0;
    };
}