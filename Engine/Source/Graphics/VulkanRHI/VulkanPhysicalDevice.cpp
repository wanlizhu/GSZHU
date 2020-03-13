#include "VulkanPhysicalDevice.h"
#include "VulkanLayerAndExtension.h"
#include <iomanip>

namespace Wanli
{
    VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice device)
        : physicalDevice(device)
    {
        vkGetPhysicalDeviceProperties(physicalDevice, &props);
        vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProps);
    
        uint32_t extCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, nullptr);
        std::vector<VkExtensionProperties> extProps(extCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, extProps.data());
        for (const auto& ext : extProps)
        {
            supportedExtensions.push_back(ext.extensionName);
        }
    }

    bool VulkanPhysicalDevice::HasExtension(const char* name) const
    {
        return std::find_if(supportedExtensions.begin(), supportedExtensions.end(), 
            [&](const char* item) {
                return strcmp(item, name) == 0;
            }) != supportedExtensions.end();
    }

    void VulkanPhysicalDevice::LogDeviceInfo() const
    {
        std::string msg;
        switch ((int)props.deviceType)
        {
        case 1:
            msg += "Integrated"; 
            break;
        case 2:
            msg += "Discrete";
            break;
        case 3:
            msg += "Virtual";
            break;
        case 4:
            msg += "CPU";
            break;
        default:
            msg += "Other";
        }

        msg += " Physical Device: ";
        msg += std::to_string(props.deviceID) + " ";

        switch (props.vendorID)
        {
        case 0x8086:
            msg += "\"Intel\"";
            break;
        case 0x10DE:
            msg += "\"Nvidia\"";
            break;
        case 0x1002:
            msg += "\"AMD\"";
            break;
        default:
            msg += "\"" + std::to_string(props.vendorID) + "\"";
        }

        msg += " " + std::string(props.deviceName) + "\n";
        uint32_t supportedVersion[3] = {
            VK_VERSION_MAJOR(props.apiVersion),
            VK_VERSION_MINOR(props.apiVersion),
            VK_VERSION_PATCH(props.apiVersion)
        };
        msg += "API Version: " 
            + std::to_string(supportedVersion[0]) 
            + "." + std::to_string(supportedVersion[1])
            + "." + std::to_string(supportedVersion[2])
            + "\n";
        msg += "Extension Count: " + std::to_string(supportedExtensions.size()) + "\n";
    
        //LOG_DEBUG(msg.c_str());
    }
    
    int VulkanPhysicalDevice::EvaluateScore() const
    {
        for (const auto& ext : VulkanLayerAndExtension::deviceExtensions)
        {
            // Returns a score of 0 if this device is missing a required extension.
            if (!HasExtension(ext))
                return 0;
        }

        int score = 0;
        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            // Adds a large score boost for discrete GPUs (dedicated graphics cards).
            score += 1000;
        }

        // Gives a higher score to devices with a higher maximum texture size.
        score += props.limits.maxImageDimension2D;
        return score;
    }
}