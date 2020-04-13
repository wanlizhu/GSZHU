#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceContextVk;

    class GISwapchainVk : public GIDeviceObjectVk
    {
        friend class GISwapchainBuilderVk;
        DECL_DEVICE_OBJECT(GISwapchainVk)
    public:
        static SharedPtr<GISwapchainVk> Create(
            SharedPtr<GIDeviceVk> device,
            void* window,
            VkExtent2D extent, /* Set { 0, 0 } to use the size of 'window' */
            uint32_t backbufferCount,
            VkFormat desiredFormat,
            VkComponentMapping componentMapping,
            VkSurfaceTransformFlagBitsKHR preTransform,
            VkSwapchainKHR oldSwapchain
        );

        virtual ~GISwapchainVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;
        
        operator const VkSwapchainKHR&() const;
        bool Resize(uint32_t width, uint32_t height);
        bool AcquireNextImage(VkFence fenceToWait, VkSemaphore semaphoreToSignal);
        bool Present(VkSemaphore semaphoreToWait);
        
        bool IsMinimized() const;
        uint32_t GetImageCount() const;
        const VkExtent2D& GetExtent() const;
        VkImageView GetCurrentImageView() const;
        const std::vector<VkImageView>& GetImageViews() const;

    protected:
        GISwapchainVk(SharedPtr<GIDeviceVk> device);
        bool RecreateSwapchain();
        void WaitImageAcquiredFence();
        void Destroy();
        
    private:
        VkSwapchainCreateInfoKHR mCreateInfoCache = {}; // For RecreateSwapchain()
        VkSwapchainKHR mSwapchainHandle = VK_NULL_HANDLE;
        VkSurfaceKHR mSurfaceHandle = VK_NULL_HANDLE;
        VkComponentMapping mComponentMapping;
        bool mAcquireNextImageAsync = true;
        bool mIsMinimized = false;

        /* Swapchain's images used by framebuffer */
        uint32_t mCurrentImageIndex = 0; // Set by vkAcquireNextImage()
        std::vector<VkImageView> mImagesViews; // The target images are created by the driver when creating VkSwapchainKHR
        VkFence mImageAcquiredFence = VK_NULL_HANDLE; // In safe-mode, we can wait this fence before returning from AcquireNextImage(...)
    };
}