#pragma once

#include "VulkanConfig.h"

namespace Wanli
{
    struct VulkanLayerAndExtension
    {
        static VkInstance instance;
        static VkPhysicalDevice physicalDevice;
        static std::vector<const char*> instanceLayers;
        static std::vector<const char*> instanceExtensions;
        static std::vector<const char*> deviceLayers;
        static std::vector<const char*> deviceExtensions;

        static void LoadFromInstanceLevel();
        static void LoadFromDeviceLevel(VkPhysicalDevice device);

        static bool HasInstanceLayer(const char* name);
        static bool HasInstanceExtension(const char* name);
        static bool HasDeviceLayer(const char* name);
        static bool HasDeviceExtension(const char* name);
    };
}