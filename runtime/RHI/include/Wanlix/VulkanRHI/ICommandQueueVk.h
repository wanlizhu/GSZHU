#pragma once

#include "Common.h"
#include "Wanlix/RHI/Object.h"

namespace Wanlix
{
    class ICommandQueueVk : public inherit_shared_from_this<Object, ICommandQueueVk>
    {
    public:
        virtual Ui64 NextFenceValue() const = 0;
        virtual Ui64 Submit(VkCommandBuffer commandBuffer) = 0;
        virtual Ui64 Submit(VkSubmitInfo const& submitInfo) = 0;
        virtual VkResult Present(VkPresentInfoKHR const& presentInfo) = 0;
        virtual VkQueue GetVkQueue() = 0;
        virtual Uint GetQueueFamilyIndex() const = 0;
        virtual Ui64 GetCompletedFenceValue() const = 0;
        virtual Ui64 WaitUntilCompleted() = 0;
        virtual void SignalFence(VkFence fence) = 0;
    };
}