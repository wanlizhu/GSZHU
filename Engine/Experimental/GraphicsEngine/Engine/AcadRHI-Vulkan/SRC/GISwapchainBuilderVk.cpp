#include "GISwapchainBuilderVk.h"
#include "GISwapchainVk.h"
#include "GIDeviceVk.h"
#include "GICommandQueueVk.h"
#include "GICommandPoolVk.h"
#include "GICommandBufferVk.h"

namespace AutoCAD::Graphics::Engine
{
    GISwapchainBuilderVk::GISwapchainBuilderVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
    {
        mComponentMapping.r = VK_COMPONENT_SWIZZLE_R;
        mComponentMapping.g = VK_COMPONENT_SWIZZLE_G;
        mComponentMapping.b = VK_COMPONENT_SWIZZLE_B;
        mComponentMapping.a = VK_COMPONENT_SWIZZLE_A;

        mCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.flags = 0;
        mCreateInfo.imageArrayLayers = 1;
        mCreateInfo.clipped = VK_TRUE;
        mCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        mCreateInfo.oldSwapchain = VK_NULL_HANDLE;
    }

    GISwapchainBuilderVk& GISwapchainBuilderVk::SetWindow(void* window)
    {
        assert(window != nullptr);

        GIPlatformVk::CreateSurface(*mDevice->GetInstance(), window, &mCreateInfo.surface);
        mDevice->SetupPresentQueue(mCreateInfo.surface);

        // If the graphics and present queues are from different queue families,
        // we either have to explicitly transfer ownership of images between
        // the queues, or we have to create the swapchain with imageSharingMode
        // as VK_SHARING_MODE_CONCURRENT
        if (mDevice->GetGraphicsQueue() == mDevice->GetPresentQueue())
        {
            mCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            mCreateInfo.queueFamilyIndexCount = 0;
            mCreateInfo.pQueueFamilyIndices = nullptr;
        }
        else
        {
            static uint32_t queueIndices[2];
            queueIndices[0] = mDevice->GetGraphicsQueue()->GetFamilyIndex();
            queueIndices[1] = mDevice->GetPresentQueue()->GetFamilyIndex();
            mCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            mCreateInfo.queueFamilyIndexCount = 2;
            mCreateInfo.pQueueFamilyIndices = queueIndices;
        }

        VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mDevice->GetPhysicalDevice(), mCreateInfo.surface, &surfaceCapabilities));

        mCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
        mCreateInfo.imageArrayLayers = 1;

        uint32_t surfaceFormatCount = 0;
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(mDevice->GetPhysicalDevice(), mCreateInfo.surface, &surfaceFormatCount, nullptr));
        std::vector<VkSurfaceFormatKHR> supportedFormats(surfaceFormatCount);
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(mDevice->GetPhysicalDevice(), mCreateInfo.surface, &surfaceFormatCount, supportedFormats.data()));
        mCreateInfo.imageFormat = supportedFormats[0].format;
        mCreateInfo.imageColorSpace = supportedFormats[0].colorSpace;

        if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
            mCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

        // vkCmdClearColorImage() command requires the image to use VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL layout
        if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            mCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

        if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT)
            mCreateInfo.imageUsage |= VK_IMAGE_USAGE_SAMPLED_BIT;

        if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
            mCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

        mCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags =
        {
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
        };

        for (const auto& compositeAlphaFlag : compositeAlphaFlags)
        {
            if (surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlag)
            {
                mCreateInfo.compositeAlpha = compositeAlphaFlag;
                break;
            }
        }

        return *this;
    }

    GISwapchainBuilderVk& GISwapchainBuilderVk::SetExtent(VkExtent2D extent, bool compulsory)
    {
        LOG_ERROR_IF(mCreateInfo.surface == NULL,
            "Have to call .SetWindow() before calling .SetColorFormat()\n");

        VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mDevice->GetPhysicalDevice(), mCreateInfo.surface, &surfaceCapabilities));

        // If the surface size is undefined, the size is set to the size of the images requested.
        if (surfaceCapabilities.currentExtent.width == 0xFFFFFFFF)
        {
            assert(mCreateInfo.imageExtent.width != (uint32_t)-1);
            mCreateInfo.imageExtent.width = min(max(mCreateInfo.imageExtent.width, surfaceCapabilities.minImageExtent.width), surfaceCapabilities.maxImageExtent.width);
            mCreateInfo.imageExtent.height = min(max(mCreateInfo.imageExtent.height, surfaceCapabilities.minImageExtent.height), surfaceCapabilities.maxImageExtent.height);
        }
        else // If the surface size is defined, the swap chain size must match.
        {
            if (compulsory)
                mCreateInfo.imageExtent = extent;
            else
                mCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
        }
        return *this;
    }

    GISwapchainBuilderVk& GISwapchainBuilderVk::AcquireNextImageAsync(bool value)
    {
        mAcquireNextImageAsync = value;
        return *this;
    }

    GISwapchainBuilderVk& GISwapchainBuilderVk::EnableVsync(bool value)
    {
        LOG_ERROR_IF(mCreateInfo.surface == NULL,
            "Have to call .SetWindow() before calling .SetColorFormat()\n");

        uint32_t physicalPresentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(mDevice->GetPhysicalDevice(), mCreateInfo.surface, &physicalPresentModeCount, nullptr);
        std::vector<VkPresentModeKHR> physicalPresentModes(physicalPresentModeCount);
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(mDevice->GetPhysicalDevice(), mCreateInfo.surface, &physicalPresentModeCount, physicalPresentModes.data()));

        VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        if (value)
        {
            // The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
            // This mode waits for the vertical blank ("v-sync")
            presentMode = VK_PRESENT_MODE_FIFO_KHR;
        }
        else
        {
            // If v-sync is NOT requested, try to find a mailbox mode
            // It's the lowest latency non-tearing present mode available
            for (const auto& currentMode : physicalPresentModes)
            {
                if (currentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                {
                    presentMode = currentMode;
                    break;
                }

                if (presentMode != VK_PRESENT_MODE_MAILBOX_KHR &&
                    currentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
                {
                    presentMode = currentMode;
                    break;
                }
            }
        }

        mCreateInfo.presentMode = presentMode;

        return *this;
    }

    GISwapchainBuilderVk& GISwapchainBuilderVk::SetBackbufferCount(uint32_t count)
    {
        LOG_ERROR_IF(mCreateInfo.surface == NULL,
            "Have to call .SetWindow() before calling .SetColorFormat()\n");

        VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mDevice->GetPhysicalDevice(), mCreateInfo.surface, &surfaceCapabilities));

        if (count < surfaceCapabilities.minImageCount)
        {
            LOG_WARNING("Request back buffer count (%d) is smaller than the minimal image count(%d) supported by the surface, reset back to the (%d).\n",
                count,
                surfaceCapabilities.minImageCount,
                surfaceCapabilities.minImageCount);
            count = surfaceCapabilities.minImageCount;
        }

        if (count > surfaceCapabilities.maxImageCount)
        {
            LOG_WARNING("Request back buffer count (%d) is greater than the maximal image count(%d) supported by the surface, reset back to the (%d).\n",
                count,
                surfaceCapabilities.maxImageCount,
                surfaceCapabilities.maxImageCount);
            count = surfaceCapabilities.maxImageCount;
        }

        mCreateInfo.minImageCount = count;

        return *this;
    }

    GISwapchainBuilderVk& GISwapchainBuilderVk::SetColorFormat(VkFormat format, bool compulsory)
    {
        LOG_ERROR_IF(mCreateInfo.surface == NULL,
            "Have to call .SetWindow() before calling .SetColorFormat()\n");

        VkSurfaceFormatKHR surfaceFormat = {};
        uint32_t surfaceFormatCount = 0;
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(mDevice->GetPhysicalDevice(), mCreateInfo.surface, &surfaceFormatCount, nullptr));
        std::vector<VkSurfaceFormatKHR> supportedFormats(surfaceFormatCount);
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(mDevice->GetPhysicalDevice(), mCreateInfo.surface, &surfaceFormatCount, supportedFormats.data()));

        if ((surfaceFormatCount == 1) && (supportedFormats[0].format == VK_FORMAT_UNDEFINED))
        {
            // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
            // the surface has no preferred format.  Otherwise, at least one
            // supported format will be returned.
            surfaceFormat.format = format;
            surfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        }
        else
        {
            // Iterate over the list of available surface format and
            // check for the presence of mCreateInfo.colorFormat(usually, VK_FORMAT_B8G8R8A8_UNORM)
            bool foundRequiredFormat = false;
            for (auto& currentFormat : supportedFormats)
            {
                if (currentFormat.format == format)
                {
                    surfaceFormat.format = currentFormat.format;
                    surfaceFormat.colorSpace = currentFormat.colorSpace;
                    foundRequiredFormat = true;
                    break;
                }
            }

            // In case mCreateInfo.colorFormat(usually, VK_FORMAT_B8G8R8A8_UNORM) is not available
            // select the first available color format
            if (!foundRequiredFormat)
            {
                surfaceFormat.format = supportedFormats[0].format;
                surfaceFormat.colorSpace = supportedFormats[0].colorSpace;

                if (!compulsory)
                {
                    LOG_WARNING("Specified format '%s' is not supported by the surface and will be replaced by '%s'\n",
                        GIFormatToStringVk(format),
                        GIFormatToStringVk(surfaceFormat.format));
                }
                else
                {
                    LOG_ERROR("Failed to find compulsory color format '%s'\n",
                        GIFormatToStringVk(format));
                }
            }
        }

        mCreateInfo.imageFormat = surfaceFormat.format;
        mCreateInfo.imageColorSpace = surfaceFormat.colorSpace;

        return *this;
    }

    GISwapchainBuilderVk& GISwapchainBuilderVk::SetPreTransform(VkSurfaceTransformFlagBitsKHR preTransform)
    {
        LOG_ERROR_IF(mCreateInfo.surface == NULL,
            "Have to call .SetWindow() before calling .SetColorFormat()\n");

        VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mDevice->GetPhysicalDevice(), mCreateInfo.surface, &surfaceCapabilities));

        mCreateInfo.preTransform = preTransform;

        return *this;
    }

    GISwapchainBuilderVk& GISwapchainBuilderVk::SetComponentMapping(VkComponentMapping componentMapping)
    {
        mComponentMapping = componentMapping;
        return *this;
    }

    GISwapchainBuilderVk& GISwapchainBuilderVk::SetOldSwapchain(VkSwapchainKHR oldSwapchain)
    {
        mCreateInfo.oldSwapchain = oldSwapchain;
        return *this;
    }

    SharedPtr<GISwapchainVk> GISwapchainBuilderVk::Build()
    {
        auto swapchain = SharedPtr<GISwapchainVk>(new GISwapchainVk(
            mDevice,
            mAcquireNextImageAsync,
            mComponentMapping,
            mCreateInfo));
        assert(swapchain->IsValid());

        return swapchain;
    }
}