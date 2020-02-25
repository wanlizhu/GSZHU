#include "VulkanPhysicalDevice.h"

namespace Wanli
{
    VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice physicalDevice)
        : PhysicalDevice(physicalDevice)
    {
        vkGetPhysicalDeviceProperties(PhysicalDevice, &Props);
        vkGetPhysicalDeviceFeatures(PhysicalDevice, &Features);
        vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &MemoryProps);
    }
    
    int VulkanPhysicalDevice::EvaluateRank() const
    {
        return 0;
    }
}