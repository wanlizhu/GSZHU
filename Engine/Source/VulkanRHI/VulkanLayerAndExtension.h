#pragma once

#include "VulkanConfig.h"

namespace Wanli
{
    struct VulkanLayerAndExtension final
    {
        static VkInstance Instance;
        static VkPhysicalDevice PhysicalDevice;
        static std::vector<const char*> InstanceLayers;
        static std::vector<const char*> InstanceExtensions;
        static std::vector<const char*> DeviceLayers;
        static std::vector<const char*> DeviceExtensions;

        static void LoadFromInstanceLevel();
        static void LoadFromDeviceLevel(VkPhysicalDevice physicalDevice);

        static bool HasInstanceLayer(const char* name);
        static bool HasInstanceExtension(const char* name);
        static bool HasDeviceLayer(const char* name);
        static bool HasDeviceExtension(const char* name);
    };
}