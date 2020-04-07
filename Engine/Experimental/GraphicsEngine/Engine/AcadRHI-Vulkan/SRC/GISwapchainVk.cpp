#include "GISwapchainVk.h"
#include "GIDeviceVk.h"
#include "GICommandQueueVk.h"

namespace AutoCAD::Graphics::Engine
{
    GISwapchainVk::GISwapchainVk(
        SharedPtr<GIDeviceVk> device,
        bool acquireNextImageAsync,
        VkComponentMapping componentMapping,
        const VkSwapchainCreateInfoKHR& createInfo
    )
        : GIDeviceObjectVk(device)
        , mAcquireNextImageAsync(acquireNextImageAsync)
        , mSurfaceHandle(createInfo.surface)
        , mComponentMapping(componentMapping)
        , mCreateInfoCache(createInfo)
    {
        RecreateSwapchain();
    }

	GISwapchainVk::~GISwapchainVk()
	{
        Destroy();
	}

    bool GISwapchainVk::IsValid() const
    {
        return mSwapchainHandle != VK_NULL_HANDLE &&
            mImagesViews.size() > 0 &&
            mDevice != nullptr;
    }

    void GISwapchainVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            (void*)mSwapchainHandle, 
            VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT, 
            name
        );
    }

    void GISwapchainVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            (void*)mSwapchainHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT,
            tag
        );
    }

    GISwapchainVk::operator const VkSwapchainKHR& () const
    {
        return mSwapchainHandle;
    }

    /* Applications should not rely on vkAcquireNextImageKHR blocking in order to
     * meter their rendering speed. The implementation may return from this function
     * immediately regardless of how many presentation requests are queued, and regardless
     * of when queued presentation requests will complete relative to the call. Instead,
     * applications can use fence to meter their frame generation work to match the
     * presentation rate.
     *
     * vkAcquireNextImageKHR(...) When successful, vkAcquireNextImageKHR acquires a presentable image
     * from swapchain that an application can use, and sets pImageIndex to the index of that image within the swapchain.
     * The presentation engine may not have finished reading from the image at the time it is acquired,
     * so the application must use semaphore and/or fence to ensure that the image layout and contents
     * are not modified until the presentation engine reads have completed.
     * The order in which images are acquired is implementation-dependent,
     * and may be different than the order the images were presented.
     *
     * VK_SUBOPTIMAL_KHR is returned from vkAcquireNextImageKHR(...) if an image became available,
     * and the swapchain no longer matches the surface properties exactly,
     * but can still be used to present to the surface successfully.
     */
    bool GISwapchainVk::AcquireNextImage(VkFence fenceToWait, VkSemaphore semaphoreToSignal)
    {
        if (mIsMinimized)
            return true;

        if (fenceToWait != VK_NULL_HANDLE)
        {
            if (vkGetFenceStatus(*mDevice, fenceToWait) == VK_NOT_READY)
            {
                // Wait the submitted render commands from previous frame to complete
                vkWaitForFences(*mDevice, 1, &fenceToWait, VK_TRUE, UINT64_MAX);
                vkResetFences(*mDevice, 1, &fenceToWait);
            }
        }

        // .semaphore and .fence must NOT both be equal to VK_NULL_HANDLE
        // .semaphore is enough for most cases if you don't care about 'graceful shutdown'.
        VkResult acquireResult = vkAcquireNextImageKHR(
            *mDevice,
            mSwapchainHandle,
            UINT64_MAX,
            semaphoreToSignal, 
            mImageAcquiredFence, 
            &mCurrentImageIndex);

        if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR ||
            acquireResult == VK_SUBOPTIMAL_KHR)
        {
            if (RecreateSwapchain())
            {
                return AcquireNextImage(fenceToWait, semaphoreToSignal);
            }
            else
            {
                return false;
            }
        }
        else if (acquireResult != VK_SUCCESS)
        {
            VK_CHECK(acquireResult);
            return false;
        }

        if (!mAcquireNextImageAsync)
        {
            WaitImageAcquiredFence();
        }

        return true;
    }

    bool GISwapchainVk::Present(VkSemaphore waitSemaphore)
    {
        if (mIsMinimized)
            return true;

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = nullptr;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &waitSemaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &mSwapchainHandle;
        presentInfo.pImageIndices = &mCurrentImageIndex;
        presentInfo.pResults = nullptr; // If pResults is not NULL, pResults must be a valid pointer to an array of swapchainCount VkResult values
        VkResult presentResult = vkQueuePresentKHR(*mDevice->GetPresentQueue(), &presentInfo);
        
        if (presentResult == VK_ERROR_OUT_OF_DATE_KHR ||
            presentResult == VK_SUBOPTIMAL_KHR)
        {
            return RecreateSwapchain();
        }
        else if (presentResult != VK_SUCCESS)
        {
            VK_CHECK(presentResult);
            return false;
        }

        return true;
    }

    bool GISwapchainVk::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0)
        {
            mIsMinimized = true;
            return true;
        }

        if (mCreateInfoCache.imageExtent.width != width &&
            mCreateInfoCache.imageExtent.height != height)
        {
            LOG_DEBUG("Resize swapchain from [%d, %d] to [%d, %d].\n",
                mCreateInfoCache.imageExtent.width,
                mCreateInfoCache.imageExtent.height,
                width,
                height);
            mCreateInfoCache.imageExtent.width = width;
            mCreateInfoCache.imageExtent.height = height;
        }

        RecreateSwapchain(); 

        return IsValid();
    }

    bool GISwapchainVk::IsMinimized() const
    {
        return mIsMinimized;
    }

    uint32_t GISwapchainVk::GetImageCount() const
    {
        return (uint32_t)mImagesViews.size();
    }

    const VkExtent2D& GISwapchainVk::GetExtent() const
    {
        return mCreateInfoCache.imageExtent;
    }

    VkImageView GISwapchainVk::GetCurrentImageView() const
    {
        return mImagesViews[mCurrentImageIndex];
    }

    const std::vector<VkImageView>& GISwapchainVk::GetImageViews() const
    {
        return mImagesViews;
    }

    void GISwapchainVk::WaitImageAcquiredFence()
    {
        if (mImageAcquiredFence != VK_NULL_HANDLE)
        {
            if (vkGetFenceStatus(*mDevice, mImageAcquiredFence) == VK_NOT_READY)
            {
                vkWaitForFences(*mDevice, 1, &mImageAcquiredFence, VK_TRUE, UINT64_MAX);
                vkResetFences(*mDevice, 1, &mImageAcquiredFence);
            }
        }
    }

    void GISwapchainVk::Destroy()
    {
        if (IsValid())
        {
            // We need to explicitly wait for all submitted Image Acquired Fences to signal.
            // Just idling the GPU is not enough and results in validation warnings.
            // As a matter of fact, it is only required to check the fence status.
            mDevice->WaitIdle();
            WaitImageAcquiredFence();

            if (mImageAcquiredFence)
            {
                vkDestroyFence(*mDevice, mImageAcquiredFence, nullptr);
                mImageAcquiredFence = VK_NULL_HANDLE;
            }

            for (size_t i = 0; i < mImagesViews.size(); i++)
            {
                vkDestroyImageView(*mDevice, mImagesViews[i], nullptr);
            }
            mImagesViews.clear();
            mCurrentImageIndex = 0;
            mIsMinimized = false;

            vkDestroySwapchainKHR(*mDevice, mSwapchainHandle, nullptr);
            mSwapchainHandle = VK_NULL_HANDLE;
        }
    }

    bool GISwapchainVk::RecreateSwapchain()
    {
        if (IsValid())
        {
            mDevice->WaitIdle();
            WaitImageAcquiredFence();
            Destroy();
        }

        VK_CHECK(vkCreateSwapchainKHR(*mDevice, &mCreateInfoCache, nullptr, &mSwapchainHandle));

        uint32_t imageCount = 0;
        vkGetSwapchainImagesKHR(*mDevice, mSwapchainHandle, &imageCount, nullptr);
        std::vector<VkImage> images(imageCount);
        vkGetSwapchainImagesKHR(*mDevice, mSwapchainHandle, &imageCount, images.data());

        mImagesViews.resize(imageCount);

        for (size_t i = 0; i < images.size(); i++)
        {
            VkImageViewCreateInfo imageViewCI = {};
            imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCI.pNext = NULL;
            imageViewCI.flags = 0;
            imageViewCI.image = images[i];
            imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCI.format = mCreateInfoCache.imageFormat;
            imageViewCI.components = mComponentMapping;
            imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCI.subresourceRange.baseMipLevel = 0;
            imageViewCI.subresourceRange.levelCount = 1;
            imageViewCI.subresourceRange.baseArrayLayer = 0;
            imageViewCI.subresourceRange.layerCount = 1;
            VK_CHECK(vkCreateImageView(*mDevice, &imageViewCI, nullptr, &mImagesViews[i]));
        }

        if (!mAcquireNextImageAsync)
        {
            VkFenceCreateInfo fenceCI = {};
            fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceCI.pNext = nullptr;
            fenceCI.flags = 0;
            VK_CHECK(vkCreateFence(*mDevice, &fenceCI, nullptr, &mImageAcquiredFence));
        }

        return IsValid();
    }

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