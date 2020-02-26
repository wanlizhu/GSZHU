#pragma once

#include "VulkanConfig.h"

namespace Wanli
{
    struct VulkanInstance;
    struct VulkanPhysicalDevice;

    struct VulkanSurface
    {
        VulkanInstance* instance = nullptr;
        VulkanPhysicalDevice* physicalDevice = nullptr;

        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VkSurfaceCapabilitiesKHR capabilities = {};
        VkSurfaceFormatKHR surfaceFormat = {};

        VulkanSurface(
            VulkanInstance* instance,
            VulkanPhysicalDevice* physicalDevice);
        ~VulkanSurface();

        operator const VkSurfaceKHR& () const { return surface; }
    };
}