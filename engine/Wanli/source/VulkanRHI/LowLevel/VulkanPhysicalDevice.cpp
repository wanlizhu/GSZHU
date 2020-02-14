#include "VulkanRHI/LowLevel/VulkanPhysicalDevice.h"
#include "VulkanRHI/LowLevel/VulkanInstance.h"
#include "Utilities/Log.h"
#include "VulkanRHI/LowLevel/VulkanLogicalDevice.h"
#include <map>

namespace Wanli
{
    static const std::vector<VkSampleCountFlagBits> kStageFlagBits =
    {
        VK_SAMPLE_COUNT_64_BIT,
        VK_SAMPLE_COUNT_32_BIT,
        VK_SAMPLE_COUNT_16_BIT,
        VK_SAMPLE_COUNT_8_BIT,
        VK_SAMPLE_COUNT_4_BIT,
        VK_SAMPLE_COUNT_2_BIT
    };

    VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanInstance* instance)
        : mVkInstance(instance->shared_from_this())
    {}

    VulkanPhysicalDevice::~VulkanPhysicalDevice()
    {
        Destroy();
    }

    bool VulkanPhysicalDevice::Initialize()
    {
        try
        {
            ChoosePhysicalDevice();
            LoadSupportedLayersAndExtensions();
            LoadMaxSampleCount();

            vkGetPhysicalDeviceProperties(mPhysicalDevice, &mProperties);
            vkGetPhysicalDeviceFeatures(mPhysicalDevice, &mSupportedFeatures);
            vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &mMemoryProperties);
        }
        catch (VulkanError & e)
        {
            LOG_ERROR("!!!Failed to initialize VulkanPhysicalDevice!!!:\n");
            LOG_ERROR("\t%s\n", e.what());
            return false;
        }
        return true;
    }

    void VulkanPhysicalDevice::Destroy()
    {
        // VkPhysicalDevice can't be destroyed
        mPhysicalDevice = VK_NULL_HANDLE;
        mProperties = {};
        mSupportedFeatures = {};
        mMemoryProperties = {};
    }

    int VulkanPhysicalDevice::RankPhysicalDevice(VkPhysicalDevice device)
    {
        int score = 0;

        // Checks if the requested extensions are supported.
        Uint propCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &propCount, nullptr);
        Array<VkExtensionProperties> extensionProps(propCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &propCount, extensionProps.data());

        for (const char* required : VulkanLogicalDevice::kDeviceExtensions)
        {
            if (std::find_if(extensionProps.begin(), extensionProps.end(),
                [&] (const VkExtensionProperties& extension) {
                    return strcmp(required, extension.extensionName) == 0;
                }) == extensionProps.end())
            {
                return 0;
            }
        }

        VkPhysicalDeviceProperties properties = {};
        VkPhysicalDeviceFeatures features = {};
        vkGetPhysicalDeviceProperties(device, &properties);
        vkGetPhysicalDeviceFeatures(device, &features);

        LogPhysicalDevice(properties, extensionProps);

        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        score += properties.limits.maxImageDimension2D;
        
        return score;
    }

    void VulkanPhysicalDevice::ChoosePhysicalDevice()
    {
        Uint deviceCount = 0;
        vkEnumeratePhysicalDevices(*mVkInstance, &deviceCount, nullptr);
        Array<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(*mVkInstance, &deviceCount, devices.data());

        std::multimap<int, VkPhysicalDevice> rankedDevices;
        
        for (const auto& device : devices)
        {
            rankedDevices.emplace(RankPhysicalDevice(device), device);
        }

        if (rankedDevices.rbegin()->first > 0)
        {
            mPhysicalDevice = rankedDevices.rbegin()->second;
        }

        assert(mPhysicalDevice != VK_NULL_HANDLE);

        LOG_INFO("Using physical device: %d (%s)\n", mProperties.deviceID, mProperties.deviceName);
    }

    void VulkanPhysicalDevice::LoadSupportedLayersAndExtensions()
    {
        Uint extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensionCount, nullptr);
        Array<VkExtensionProperties> extensionProps(extensionCount);
        vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensionCount, extensionProps.data());
        for (const auto& prop : extensionProps)
        {
            mSupportedExtensions.push_back(prop.extensionName);
        }

        Uint layerCount = 0;
        vkEnumerateDeviceLayerProperties(mPhysicalDevice, &layerCount, nullptr);
        Array<VkLayerProperties> layerProps(layerCount);
        vkEnumerateDeviceLayerProperties(mPhysicalDevice, &layerCount, layerProps.data());
        for (const auto& prop : layerProps)
        {
            mSupportedLayers.push_back(prop.layerName);
        }
    }

    void VulkanPhysicalDevice::LoadMaxSampleCount()
    {
        VkPhysicalDeviceProperties properties = {};
        vkGetPhysicalDeviceProperties(mPhysicalDevice, &properties);

        auto counts = std::min(properties.limits.framebufferColorSampleCounts,
                               properties.limits.framebufferDepthSampleCounts);
        for (const auto& sampleFlag : kStageFlagBits)
        {
            if (counts & sampleFlag)
            {
                mMSAASamples = sampleFlag;
            }
        }
    }

    void VulkanPhysicalDevice::LogPhysicalDevice(
        const VkPhysicalDeviceProperties& properties,
        const Array<VkExtensionProperties>& extensions)
    {
        switch (properties.deviceType)
        {
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                LOG_INFO("Integrated");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                LOG_INFO("Discrete");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                LOG_INFO("Virtual");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                LOG_INFO("CPU");
                break;
            default:
                LOG_INFO("Other [%d]", properties.deviceType);
        }

        LOG_INFO(" Physical Device: %d", properties.deviceID);

        switch (properties.vendorID)
        {
            case 0x8086:
                LOG_INFO(" \"Intel\"");
                break;
            case 0x10DE:
                LOG_INFO(" \"Nvidia\"");
                break;
            case 0x1002:
                LOG_INFO(" \"AMD\"");
                break;
            default:
                LOG_INFO(" \"%d\"", properties.vendorID);
        }

        LOG_INFO(" %s\n", properties.deviceName);

        Uint version[3] = {
            VK_VERSION_MAJOR(properties.apiVersion),
            VK_VERSION_MINOR(properties.apiVersion),
            VK_VERSION_PATCH(properties.apiVersion)
        };
        LOG_INFO("API Version: %d.%d.%d\n", version[0], version[1], version[2]);

        LOG_INFO("Extensions:");
        for (const auto& prop : extensions)
        {
            LOG_INFO("\t%s\n", prop.extensionName);
        }

        LOG_INFO("\n");
    }
}