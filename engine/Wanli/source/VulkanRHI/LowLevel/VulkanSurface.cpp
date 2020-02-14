#include "VulkanRHI/LowLevel/VulkanSurface.h"
#include "VulkanRHI/LowLevel/VulkanInstance.h"
#include "VulkanRHI/LowLevel/VulkanPhysicalDevice.h"
#include "VulkanRHI/VulkanError.h"
#include "VulkanRHI/LowLevel/VulkanDebug.h"
#include "Utilities/Log.h"
#include "Engine.h"

namespace Wanli
{
    VulkanSurface::VulkanSurface(VulkanInstance* instance,
                                 VulkanPhysicalDevice* physicalDevice)
        : mVkInstance(instance->shared_from_this())
        , mPhysicalDevice(physicalDevice->shared_from_this())
    {}

    VulkanSurface::~VulkanSurface()
    {
        Destroy();
    }

    bool VulkanSurface::Initialize()
    {
        try
        {
            mSurface = VulkanPlatform::CreateSurface(*mVkInstance);
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*mPhysicalDevice, mSurface, &mCapabilities);

            Uint formatCount = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(*mPhysicalDevice, mSurface, &formatCount, nullptr);
            Array<VkSurfaceFormatKHR> surfaceFormats(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(*mPhysicalDevice, mSurface, &formatCount, surfaceFormats.data());

            if ((formatCount == 1) && 
                (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
            {
                mFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
                mFormat.colorSpace = surfaceFormats[0].colorSpace;
            }
            else
            {
                bool foundIdeal = false;
                for (auto& format : surfaceFormats)
                {
                    if (format.format == VK_FORMAT_B8G8R8A8_UNORM)
                    {
                        mFormat.format = format.format;
                        mFormat.colorSpace = format.colorSpace;
                        foundIdeal = true;
                        break;
                    }
                }

                // In case VK_FORMAT_B8G8R8A8_UNORM is not available
                // select the first available color format
                if (!foundIdeal)
                {
                    mFormat.format = surfaceFormats[0].format;
                    mFormat.colorSpace = surfaceFormats[0].colorSpace;
                }
            }
        }
        catch (VulkanError & e)
        {
            LOG_ERROR("%s\n", e.what());
            return false;
        }
        return true;
    }

    void VulkanSurface::Destroy()
    {
        if (mSurface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(*mVkInstance, mSurface, nullptr);
            mSurface = VK_NULL_HANDLE;
        }
    }
}