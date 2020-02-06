#pragma once

#include "VulkanRHI.h"

namespace Wanli
{
    class RHIQueue;
    class RHIFencePool;
    class RHIQueryPool;
    class RHIResourcePool;

    class DLLDECL RHIDevice : public std::enable_shared_from_this<RHIDevice>
    {
    public:
        RHIDevice(VkPhysicalDevice device);
        virtual ~RHIDevice();

        bool Initialize(const PCCHArray& requiredExtensions);
        bool CreatePresentQueue(VkSurfaceKHR surface);
        void Destroy();

        bool IsFormatSupported(VkFormat format) const;
        VkComponentMapping GetFormatComponentMapping(EPixelFormat format) const;
        
        inline VkDevice GetHandle() const { return mDevice; }
        inline VkPhysicalDevice GetPhysicalHandle() const { return mPhysicalDevice; }
        inline const VkPhysicalDeviceProperties& GetDeviceProperties() const { return mPhysicalDeviceProperties; }
        inline const VkPhysicalDeviceLimits& GetDeviceLimits() const { return mPhysicalDeviceProperties.limits; }
        inline const VkFormatProperties* GetFormatPropertiesArray() const { return mFormatPropertiesArray; }
        
        inline RHIFencePool* GetFencePool() const { return mFencePool.get(); }
        inline RHIQueryPool* GetQueryPool() const { return mQueryPool.get(); }
        inline RHIResourcePool* GetResourcePool() const { return mResourcePool.get(); }

        inline RHIQueue* GetGraphicsQueue() const { return mGraphicsQueue; }
        inline RHIQueue* GetComputeQueue() const { return mComputeQueue; }
        inline RHIQueue* GetTransferQueue() const { return mTransferQueue; }
        inline RHIQueue* GetPresentQueue() const { return mPresentQueue; }
        
    protected:
        void CreateLogicalDevice();
        void CreatePools();
        void CreateQueues();
        void GetDeviceExtensionAndLayers(PCCHArray& extensions, PCCHArray& layers);

    private:
        VkDevice mDevice;
        VkPhysicalDevice mPhysicalDevice;
        VkPhysicalDeviceProperties mPhysicalDeviceProperties;
        VkPhysicalDeviceFeatures mPhysicalDeviceFeatures;
        Array<VkQueueFamilyProperties> mQueueFamilyPropertiesArray;

        VkFormatProperties mFormatPropertiesArray[VK_FORMAT_RANGE_SIZE];
        HashMap<VkFormat, VkFormatProperties> mExtensionFormatPropertiesMap;
        VkComponentMapping mPixelComponentMappingArray[PF_MAX];

        RHIQueue* mGraphicsQueue = nullptr;
        RHIQueue* mComputeQueue = nullptr;
        RHIQueue* mTransferQueue = nullptr;
        RHIQueue* mPresentQueue = nullptr;

        UniquePtr<RHIFencePool> mFencePool;
        UniquePtr<RHIQueryPool> mQueryPool;
        UniquePtr<RHIResourcePool> mResourcePool;
    };
}