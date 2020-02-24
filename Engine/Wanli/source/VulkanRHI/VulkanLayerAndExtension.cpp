#include "VulkanLayerAndExtension.h"
#include <algorithm>

namespace Wanli
{
    VkInstance VulkanLayerAndExtension::Instance = VK_NULL_HANDLE;
    VkPhysicalDevice VulkanLayerAndExtension::PhysicalDevice = VK_NULL_HANDLE;
    std::vector<const char*> VulkanLayerAndExtension::InstanceLayers = {};
    std::vector<const char*> VulkanLayerAndExtension::InstanceExtensions = {};
    std::vector<const char*> VulkanLayerAndExtension::DeviceLayers = {};
    std::vector<const char*> VulkanLayerAndExtension::DeviceExtensions = {};

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
            InstanceLayers.resize(count);
            std::vector<VkLayerProperties> props(count);
            vkEnumerateInstanceLayerProperties(&count, props.data());
            for (const auto& prop : props)
            {
                InstanceLayers.push_back(prop.layerName);
            }
        }

        count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
        if (count)
        {
            InstanceExtensions.reserve(count);
            std::vector<VkExtensionProperties> props(count);
            vkEnumerateInstanceExtensionProperties(nullptr, &count, props.data());
            for (const auto& prop : props)
            {
                InstanceExtensions.push_back(prop.extensionName);
            }

            for (const auto& layer : InstanceLayers)
            {
                count = 0;
                vkEnumerateInstanceExtensionProperties(layer, &count, nullptr);
                if (count)
                {
                    std::vector<VkExtensionProperties> props(count);
                    vkEnumerateInstanceExtensionProperties(layer, &count, props.data());
                    for (const auto& prop : props)
                    {
                        InstanceExtensions.push_back(prop.extensionName);
                    }
                }
            }
        }

        UniqueArray(InstanceLayers);
        UniqueArray(InstanceExtensions);
        ChangeLayerLoadingOrder(InstanceLayers);
        isLoaded = true;
    }

    void VulkanLayerAndExtension::LoadFromDeviceLevel(VkPhysicalDevice physicalDevice)
    {
        static bool isLoaded = false;
        if (isLoaded)
            return;

        if (physicalDevice == VK_NULL_HANDLE)
            return;

        PhysicalDevice = physicalDevice;
        uint32_t count;

        count = 0;
        vkEnumerateDeviceLayerProperties(PhysicalDevice, &count, nullptr);
        if (count)
        {
            uint32_t deviceLayerCount = count;
            DeviceLayers.resize(count);
            std::vector<VkLayerProperties> props(count);
            vkEnumerateDeviceLayerProperties(physicalDevice, &count, props.data());
            for (const auto& prop : props)
            {
                DeviceLayers.push_back(prop.layerName);
            }

            // validation layer for device only report out for one layer,
            // but it seems we could ask for all layers that asked for instance
            // so we just add them all in brutally
            // assume all instance layers are also implemented for device layers
            if (deviceLayerCount == 1)
            {
                DeviceLayers.clear();
                DeviceLayers = InstanceLayers;
            }
        }

        count = 0;
        vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &count, nullptr);
        if (count)
        {
            DeviceExtensions.reserve(count);
            std::vector<VkExtensionProperties> props(count);
            vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &count, props.data());
            for (const auto& prop : props)
            {
                DeviceExtensions.push_back(prop.extensionName);
            }

            for (const auto& layer : DeviceLayers)
            {
                count = 0;
                vkEnumerateDeviceExtensionProperties(PhysicalDevice, layer, &count, nullptr);
                if (count)
                {
                    std::vector<VkExtensionProperties> props(count);
                    vkEnumerateDeviceExtensionProperties(PhysicalDevice, layer, &count, props.data());
                    for (const auto& prop : props)
                    {
                        DeviceExtensions.push_back(prop.extensionName);
                    }
                }
            }
        }

        UniqueArray(DeviceLayers);
        UniqueArray(DeviceExtensions);
        ChangeLayerLoadingOrder(DeviceLayers);
        isLoaded = true;
    }

    bool VulkanLayerAndExtension::HasInstanceLayer(const char* name)
    {
        return std::find_if(InstanceLayers.begin(), InstanceLayers.end(), 
            [&](const char* item) {
                return strcmp(item, name) == 0;
            }) != InstanceLayers.end();
    }

    bool VulkanLayerAndExtension::HasInstanceExtension(const char* name)
    {
        return std::find_if(InstanceExtensions.begin(), InstanceExtensions.end(),
            [&](const char* item) {
                return strcmp(item, name) == 0;
            }) != InstanceExtensions.end();
    }

    bool VulkanLayerAndExtension::HasDeviceLayer(const char* name)
    {
        return std::find_if(DeviceLayers.begin(), DeviceLayers.end(),
            [&](const char* item) {
                return strcmp(item, name) == 0;
            }) != DeviceLayers.end();
    }

    bool VulkanLayerAndExtension::HasDeviceExtension(const char* name)
    {
        return std::find_if(DeviceExtensions.begin(), DeviceExtensions.end(),
            [&](const char* item) {
                return strcmp(item, name) == 0;
            }) != DeviceExtensions.end();
    }
}