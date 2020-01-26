#pragma once

#include "RHI/IBufferView.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class IBufferViewVk : public IBufferView
    {
    public:
        virtual VkBufferView GetVkBufferView() const = 0;
    };
}