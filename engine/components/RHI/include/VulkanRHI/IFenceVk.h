#pragma once

#include "RHI/IFence.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class IFenceVk : public IFence
    {
    public:
        virtual VkFence GetVkFence() const = 0;
    };
}