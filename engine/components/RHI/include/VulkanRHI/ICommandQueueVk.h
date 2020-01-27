#pragma once

#include "RHI/ICommandQueue.h"
#include "VulkanRHI/VkUtilities.h"
#include "VulkanRHI/IDeviceVk.h"

namespace Wanlix
{
    class ICommandQueueVk : public ICommandQueue
    {
    public:
        virtual Uint64 Submit(VkCommandBuffer cmdBuffer) = 0;
        virtual Uint64 Submit(const VkSubmitInfo& submitInfo) = 0;
        virtual VkResult Present(const VkPresentInfoKHR& presentInfo) = 0;
        virtual VkQueue GetVkQueue() = 0;
    };
}