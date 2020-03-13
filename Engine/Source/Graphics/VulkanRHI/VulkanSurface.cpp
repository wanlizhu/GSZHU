#include "VulkanSurface.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"

namespace Wanli
{
    VulkanSurface::VulkanSurface(
        VulkanInstance* instance,
        VulkanPhysicalDevice* device)
        : instance(instance)
        , physicalDevice(device)
        , surface(VK_NULL_HANDLE)
    {
        // Creates the surface.
        // ...

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*physicalDevice, surface, &capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(*physicalDevice, surface, &formatCount, nullptr);
        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(*physicalDevice, surface, &formatCount, formats.data());

        if ((formatCount == 1) && formats[0].format == VK_FORMAT_UNDEFINED)
        {
            surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
            surfaceFormat.colorSpace = formats[0].colorSpace;
        }
        else
        {
            // Iterate over the list of available surface format and
            // check for the presence of VK_FORMAT_B8G8R8A8_UNORM
            auto it = std::find_if(formats.begin(), formats.end(),
                [&](const VkSurfaceFormatKHR& item) {
                    if (item.format == VK_FORMAT_B8G8R8A8_UNORM)
                    {
                        surfaceFormat.format = item.format;
                        surfaceFormat.colorSpace = item.colorSpace;
                        return true;
                    }
                    return false;
                });

            // In case VK_FORMAT_B8G8R8A8_UNORM is not available
            // select the first available color format
            if (it == formats.end())
            {
                surfaceFormat.format = formats[0].format;
                surfaceFormat.colorSpace = formats[0].colorSpace;
#ifdef _DEBUG
                throw std::runtime_error("VK_FORMAT_B8G8R8A8_UNORM is not available");
#endif
            }
        }
    }

    VulkanSurface::~VulkanSurface()
    {
        if (surface)
        {
            vkDestroySurfaceKHR(*instance, surface, nullptr);
            surface = VK_NULL_HANDLE;
        }
    }
}