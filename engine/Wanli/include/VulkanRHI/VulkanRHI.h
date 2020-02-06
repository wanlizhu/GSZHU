#pragma once

#include "BasicTypes.h"
#include "RHIPixelFormat.h"
#include "RHIFlags.h"
#include "RHIPlatform.h"
#include "RHIUtilities.h"

namespace Wanli
{
    class RHIDebugContext;
    class RHIDevice;
    class RHICommandQueue;
    class RHISwapChain;

    struct VulkanRHICreateInfo
    {
        Version version = { 1, 1, 0 };
        PCCHArray requiredInstanceExtensions;
        PCCHArray requiredDeviceExtensions;
    };

    class DLLDECL VulkanRHI
    {
    public:
        bool Initialize(const VulkanRHICreateInfo& info);
        void Destroy();

        inline VkInstance GetVkInstance() const { return mVkInstance; }
        inline RHIDevice* GetDevice() const { return mDevice.get(); }
        inline const PCCHArray& GetInstanceExtensions() const { return mInstanceExtensions; }
        inline const PCCHArray& GetInstanceLayers() const { return mInstanceLayers; }
        
        inline RHISwapChain* GetSwapChain() const { return mSwapChain.get(); }
        inline const Array<VkImage>& GetBackbufferImages() const { return mBackbufferImages; }
        inline const Array<VkImageView>& GetBackbufferViews() const { return mBackbufferViews; }
        inline EPixelFormat GetPixelFormat() const { return mPixelFormat; }

    protected:
        void CreateVkInstance();
        void CreateDebugContext();
        void CreateDevice();
        void CreateSwapChain();

        void DestroySwapChain();
        void DestroyDevice();
        void DestroyDebugContext();
        void DestroyVkInstance();

    private:
        static void GetInstanceExtensionsAndLayers(PCCHArray& extensions, PCCHArray& layers);
        Uint SelectPhysicalDevice();

    private:
        VulkanRHICreateInfo mCreateInfo;

        VkInstance mVkInstance = VK_NULL_HANDLE;
        PCCHArray mInstanceExtensions;
        PCCHArray mInstanceLayers;
        
        SharedPtr<RHIDevice> mDevice;
        UniquePtr<RHIDebugContext> mDebugContext;

        SharedPtr<RHISwapChain> mSwapChain;
        EPixelFormat mPixelFormat = PF_B8G8R8A8;
        Array<VkImage> mBackbufferImages;
        Array<VkImageView> mBackbufferViews;
    };
}