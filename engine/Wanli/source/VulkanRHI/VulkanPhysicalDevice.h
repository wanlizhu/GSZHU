#pragma once

#include "VulkanConfig.h"

namespace Wanli
{
    class VulkanPhysicalDevice final
    {
    public:
        VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties Props = {};
        VkPhysicalDeviceFeatures Features = {};
        VkPhysicalDeviceMemoryProperties MemoryProps = {};

        VulkanPhysicalDevice(VkPhysicalDevice physicalDevice);
        int EvaluateRank() const;
    };
}