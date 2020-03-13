#pragma once

#include "VulkanConfig.h"

namespace Wanli
{
    struct VulkanPhysicalDevice
    {
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties props = {};
        VkPhysicalDeviceFeatures supportedFeatures = {};
        VkPhysicalDeviceMemoryProperties memoryProps = {};
        std::vector<const char*> supportedExtensions = {};

        VulkanPhysicalDevice(VkPhysicalDevice device);
        ~VulkanPhysicalDevice();

        inline operator const VkPhysicalDevice&() const { return physicalDevice; }
        int EvaluateScore() const;
        bool HasExtension(const char* name) const;
        void LogDeviceInfo() const;
    };
}