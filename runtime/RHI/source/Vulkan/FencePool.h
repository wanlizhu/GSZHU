#pragma once

#include "LogicalDevice.h"

namespace Wanlix
{
    class FencePool : public Object
    {
        CLASSINFO(FencePool)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        static SharedPtr<FencePool> Create(SharedPtr<LogicalDevice> device);

        ~FencePool();
        VulkanObject<VkFence> GetFence();
        void FreeFence(VulkanObject<VkFence>&& fence);

    private:
        FencePool(SharedPtr<LogicalDevice> device);

    private:
        SharedPtr<LogicalDevice> mLogicalDevice;
        Array<VulkanObject<VkFence>> mFences;
    };
}