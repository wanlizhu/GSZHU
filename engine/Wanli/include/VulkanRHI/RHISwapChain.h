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
        enum class EStatus 
        {
            Normal = 0,
            OutOfData = 1,
            SurfaceLost = 2,
        };

        RHISwapChain(VkInstance instance,
                     RHIDevice* device,
                     Uint width,
                     Uint height,
                     bool vsync);
        virtual ~RHISwapChain();

        void GetBackBuffers(EPixelFormat& pixelFormat, Array<VkImage>& images) const;
        Uint AcquireNextImage(VkSemaphore* semaphore);

        virtual void Destroy() override;
        inline bool IsLockToVsync() const { return mLockToVsync; }
        inline VkSwapchainKHR GetHandle() const { return mSwapChain; }
        inline Uint GetWidth() const { return mSwapChainCreateInfo.imageExtent.width; }
        inline Uint GetHeight() const { return mSwapChainCreateInfo.imageExtent.height; }
        inline Uint GetBackBufferCount() const { return mBackBufferCount; }
        inline const VkSwapchainCreateInfoKHR& GetCreateInfo() const { return mSwapChainCreateInfo; }
        inline VkFormat GetColorFormat() const { return mColorFormat; }

    private:
        VkInstance mVkInstance;
        VkSwapchainKHR mSwapChain;
        VkSurfaceKHR mSurface;
        VkSwapchainCreateInfoKHR mSwapChainCreateInfo;
        VkFormat mColorFormat;
        Uint mBackBufferCount = 0;

        Array<VkSemaphore> mImageAcquiredSemaphore;

        Uint mImageIndex = 0;
        Uint mSemaphoreIndex = 0;
        Uint mNumPresentCalls;
        Uint mNumAcquireCalls;
        bool mLockToVsync = false;
        Uint mPresentID = 0;
    };
}