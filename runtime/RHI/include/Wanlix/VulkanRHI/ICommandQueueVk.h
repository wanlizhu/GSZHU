#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/Object.h"

namespace Wanlix
{
    class RHI_API ICommandQueueVk : public Object
    {
        CLASSINFO(ICommandQueueVk)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        virtual Uint64 NextFenceValue() const = 0;
        virtual Uint64 Submit(VkCommandBuffer commandBuffer) = 0;
        virtual Uint64 Submit(VkSubmitInfo const& submitInfo) = 0;
        virtual VkResult Present(VkPresentInfoKHR const& presentInfo) = 0;
        virtual VkQueue GetVkQueue() = 0;
        virtual Uint GetQueueFamilyIndex() const = 0;
        virtual Uint64 GetCompletedFenceValue() const = 0;
        virtual Uint64 WaitUntilCompleted() = 0;
        virtual void SignalFence(VkFence fence) = 0;
    };
}