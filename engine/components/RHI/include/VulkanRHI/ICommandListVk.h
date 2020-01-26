#pragma once

#include "RHI/ICommandList.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class ICommandListVk : public ICommandList
    {
    public:
        virtual VkCommandBuffer GetVkCommandBuffer() const = 0;
    };
}