#include "VulkanLayerAndExtension.h"
#include <algorithm>

namespace Wanli
{
    VkInstance VulkanLayerAndExtension::instance = VK_NULL_HANDLE;
    VkPhysicalDevice VulkanLayerAndExtension::physicalDevice = VK_NULL_HANDLE;
    std::vector<const char*> VulkanLayerAndExtension::instanceLayers = {};
    std::vector<const char*> VulkanLayerAndExtension::instanceExtensions = {};
    std::vector<const char*> VulkanLayerAndExtension::deviceLayers = {};
    std::vector<const char*> VulkanLayerAndExtension::deviceExtensions = {};

    void ChangeLayerLoadingOrder(std::vector<const char*>& layers)
    {
        // VK_LAYER_GOOGLE_threading better to be the very first one
        // VK_LAYER_GOOGLE_unique_objects need to be after VK_LAYER_LUNARG_core_validation
        uint32_t googleThreading = 0;
        uint32_t googleUniqueObjects = 0;
        uint32_t lunargCore = 0;

        for (uint32_t i = 0; i < (uint32_t)layers.size(); i++)
        {
            if (strcmp(layers[i], "VK_LAYER_GOOGLE_threading") == 0)
            {
                googleThreading = i;
            }
            if (strcmp(layers[i], "VK_LAYER_GOOGLE_unique_objects") == 0)
            {
                googleUniqueObjects = i;
            }
            if (strcmp(layers[i], "VK_LAYER_LUNARG_core_validation") == 0)
            {
                lunargCore = i;
            }
        }

        if (googleThreading != 0)
        {
            std::swap(layers[0], layers[googleThreading]);
        }
        if (googleUniqueObjects < lunargCore)
        {
            std::swap(layers[googleUniqueObjects], layers[lunargCore]);
        }
    }

    void UniqueArray(std::vector<const char*>& names)
    {
        std::sort(names.begin(), names.end(), 
            [&](const char* lhs, const char* rhs) {
                return (bool)strcmp(lhs, rhs);
            });
        auto pos = std::unique(names.begin(), names.end());
        names.erase(pos, names.end());
    }

    void VulkanLayerAndExtension::LoadFromInstanceLevel()
    {
        static bool isLoaded = false;
        if (isLoaded)
            return;

        uint32_t count;

        count = 0;
        vkEnumerateInstanceLayerProperties(&count, nullptr);
        if (count)
        {
            instanceLayers.resize(count);
            std::vector<VkLayerProperties> props(count);
            vkEnumerateInstanceLayerProperties(&count, props.data());
            for (const auto& prop : props)
            {
                instanceLayers.push_back(prop.layerName);
            }
        }

        count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
        if (count)
        {
            instanceExtensions.reserve(count);
            std::vector<VkExtensionProperties> props(count);
            vkEnumerateInstanceExtensionProperties(nullptr, &count, props.data());
            for (const auto& prop : props)
            {
                instanceExtensions.push_back(prop.extensionName);
            }

            for (const auto& layer : instanceLayers)
            {
                count = 0;
                vkEnumerateInstanceExtensionProperties(layer, &count, nullptr);
                if (count)
                {
                    std::vector<VkExtensionProperties> props(count);
                    vkEnumerateInstanceExtensionProperties(layer, &count, props.data());
                    for (const auto& prop : props)
                    {
                        instanceExtensions.push_back(prop.extensionName);
                    }
                }
            }
        }

        UniqueArray(instanceLayers);
        UniqueArray(instanceExtensions);
        ChangeLayerLoadingOrder(instanceLayers);
        isLoaded = true;
    }

    void VulkanLayerAndExtension::LoadFromDeviceLevel(VkPhysicalDevice device)
    {
        static bool isLoaded = false;
        if (isLoaded)
            return;

        if (device == VK_NULL_HANDLE)
            return;

        physicalDevice = device;
        uint32_t count;

        count = 0;
        vkEnumerateDeviceLayerProperties(physicalDevice, &count, nullptr);
        if (count)
        {
            uint32_t deviceLayerCount = count;
            deviceLayers.resize(count);
            std::vector<VkLayerProperties> props(count);
            vkEnumerateDeviceLayerProperties(physicalDevice, &count, props.data());
            for (const auto& prop : props)
            {
                deviceLayers.push_back(prop.layerName);
            }

            // validation layer for device only report out for one layer,
            // but it seems we could ask for all layers that asked for instance
            // so we just add them all in brutally
            // assume all instance layers are also implemented for device layers
            if (deviceLayerCount == 1)
            {
                deviceLayers.clear();
                deviceLayers = instanceLayers;
            }
        }

        count = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, nullptr);
        if (count)
        {
            deviceExtensions.reserve(count);
            std::vector<VkExtensionProperties> props(count);
            vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, props.data());
            for (const auto& prop : props)
            {
                deviceExtensions.push_back(prop.extensionName);
            }

            for (const auto& layer : deviceLayers)
            {
                count = 0;
                vkEnumerateDeviceExtensionProperties(physicalDevice, layer, &count, nullptr);
                if (count)
                {
                    std::vector<VkExtensionProperties> props(count);
                    vkEnumerateDeviceExtensionProperties(physicalDevice, layer, &count, props.data());
                    for (const auto& prop : props)
                    {
                        deviceExtensions.push_back(prop.extensionName);
                    }
                }
            }
        }

        UniqueArray(deviceLayers);
        UniqueArray(deviceExtensions);
        ChangeLayerLoadingOrder(deviceLayers);
        isLoaded = true;
    }

    bool VulkanLayerAndExtension::HasInstanceLayer(const char* name)
    {
        return std::find_if(instanceLayers.begin(), instanceLayers.end(), 
            [&](const char* item) {
                return strcmp(item, name) == 0;
            }) != instanceLayers.end();
    }

    bool VulkanLayerAndExtension::HasInstanceExtension(const char* name)
    {
        return std::find_if(instanceExtensions.begin(), instanceExtensions.end(),
            [&](const char* item) {
                return strcmp(item, name) == 0;
            }) != instanceExtensions.end();
    }

    bool VulkanLayerAndExtension::HasDeviceLayer(const char* name)
    {
        return std::find_if(deviceLayers.begin(), deviceLayers.end(),
            [&](const char* item) {
                return strcmp(item, name) == 0;
            }) != deviceLayers.end();
    }

    bool VulkanLayerAndExtension::HasDeviceExtension(const char* name)
    {
        return std::find_if(deviceExtensions.begin(), deviceExtensions.end(),
            [&](const char* item) {
                return strcmp(item, name) == 0;
            }) != deviceExtensions.end();
    }
}