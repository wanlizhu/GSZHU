#pragma once

#include "RHI/ICommandList.h"
#include "VulkanRHI/VkUtilities.h"
#include "VulkanRHI/IDeviceVk.h"

namespace Wanlix
{
    class ICommandListVk : public ICommandList
    {
    public:
        virtual VkCommandBuffer GetVkCommandBuffer() const = 0;
    };
}