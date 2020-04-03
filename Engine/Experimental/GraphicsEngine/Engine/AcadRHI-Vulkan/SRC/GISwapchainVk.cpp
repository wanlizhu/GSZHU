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
}