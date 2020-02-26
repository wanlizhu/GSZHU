#pragma once

#include "VulkanConfig.h"
#include "VulkanQueue.h"

namespace Wanli
{
    struct VulkanInstance;
    struct VulkanPhysicalDevice;
    struct VulkanSurface;

    struct VulkanLogicalDevice
    {
        VulkanInstance* instance = nullptr;
        VulkanPhysicalDevice* physicalDevice = nullptr;
        VulkanSurface* surface = nullptr;

        VkDevice logicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceFeatures enabledFeatures = {};

        VkQueueFlags supportedQueues;
        VulkanQueue graphicsQueue;
        VulkanQueue presentQueue;
        VulkanQueue computeQueue;
        VulkanQueue transferQueue;

        VulkanLogicalDevice(
            VulkanInstance* instance,
            VulkanPhysicalDevice* physicalDevice,
            VulkanSurface* surface);
        ~VulkanLogicalDevice();

        inline operator const VkDevice&() const { return logicalDevice; }
    };
}