#include "VulkanRHI/LowLevel/VulkanLayer.h"

namespace Wanli
{
    VulkanInstanceLayer::VulkanInstanceLayer(const char* layerName)
    {
        Uint extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, nullptr);
        mRequiredExtensions.resize(extensionCount);
        vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, mRequiredExtensions.data());
    }

    VulkanDeviceLayer::VulkanDeviceLayer(const char* layerName, VkPhysicalDevice device)
    {
        Uint extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, layerName, &extensionCount, nullptr);
        mRequiredExtensions.resize(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, layerName, &extensionCount, mRequiredExtensions.data());
    }
}