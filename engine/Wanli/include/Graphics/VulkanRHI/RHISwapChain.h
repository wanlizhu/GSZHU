#pragma once

#include "RHIDeviceObject.h"

namespace Wanli
{
    class RHIDevice;
    class RHIQueue;
    class RHIFence;

    class DLLDECL RHISwapChain : public RHIDeviceObject
    {
    public:
        enum class EPresentStatus 
        {
            Normal = 0,
            OutOfData = 1,
            SurfaceLost = 2,
        };

        RHISwapChain(
            VkInstance instance,
            RHIDevice* device,
            VkFormat format,
            Uint width,
            Uint height,
            bool vsync
        );
        virtual ~RHISwapChain();

        virtual void Destroy() override;
        inline bool IsLockToVsync() const { return mLockToVsync; }
        inline VkInstance GetVkInstance() const { return mVkInstance; }
        inline VkSwapchainKHR GetHandle() const { return mSwapChain; }
        inline Uint GetWidth() const { return mSwapChainCreateInfo.imageExtent.width; }
        inline Uint GetHeight() const { return mSwapChainCreateInfo.imageExtent.height; }
        inline VkFormat GetColorFormat() const { return mColorFormat; }
        inline const VkSwapchainCreateInfoKHR& GetCreateInfo() const { return mSwapChainCreateInfo; }
        inline Array<VkImage>& GetImages() { return mImages; }
        inline Array<VkImageView>& GetImageViews() { return mImageViews; }
        inline VkSurfaceKHR* GetSurface() const { return mSurface; }

        Uint AcquireNextImage(VkSemaphore* semaphore);
        EPresentStatus Present(VkQueue presentQueue, VkSemaphore* complete);
        
    private:
        VkSwapchainCreateInfoKHR mSwapChainCreateInfo;
        VkInstance mVkInstance;
        VkSurfaceKHR mSurface;
        VkSwapchainKHR mSwapChain;
        VkFormat mColorFormat;
        
        Array<VkImage> mImages;
        Array<VkImageView> mImageViews;
        Array<VkSemaphore> mImageAcquireSemaphores;

        Uint mImageIndex = 0;
        Uint mSemaphoreIndex = 0;
        Uint mNumPresentCalls;
        Uint mNumAcquireCalls;
        Uint mPresentID = 0;
        bool mLockToVsync = false;
    };
}