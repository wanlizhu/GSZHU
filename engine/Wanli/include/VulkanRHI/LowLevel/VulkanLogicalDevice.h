#pragma once

#include "VulkanRHI/VulkanConfig.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    class VulkanInstance;
    class VulkanPhysicalDevice;
    class VulkanSurface;

    class DLLDECL VulkanLogicalDevice : public NonCopyable
        , public std::enable_shared_from_this<VulkanLogicalDevice>
    {
    public:
        static const PCCHArray kDeviceValidationLayers;
        static const PCCHArray kDeviceExtensions;

        VulkanLogicalDevice(VulkanInstance* instance,
                            VulkanPhysicalDevice* physicalDevice,
                            VulkanSurface* surface);
        virtual ~VulkanLogicalDevice();

        virtual bool Initialize();
        virtual void Destroy();

        inline operator const VkDevice& () const { return mLogicalDevice; }
        inline const VkPhysicalDeviceFeatures& GetEnabledFeatures() const { return mEnabledFeatures; }
        inline const PCCHArray& GetEnabledLayers() const { return mEnabledLayers; }
        inline const PCCHArray& GetEnabledExtensions() const { return mEnabledExtensions; }
        inline const VkQueue& GetGraphicsQueue() const { return mGraphicsQueue; }
        inline const VkQueue& GetPresentQueue() const { return mPresentQueue; }
        inline const VkQueue& GetComputeQueue() const { return mComputeQueue; }
        inline const VkQueue& GetTransferQueue() const { return mTransferQueue; }
        inline Uint GetGraphicsFamily() const { return mGraphicsFamily; }
        inline Uint GetPresentFamily() const { return mPresentFamily; }
        inline Uint GetComputeFamily() const { return mComputeFamily; }
        inline Uint GetTransferFamily() const { return mTransferFamily; }

        void EnableExtension(const char* extension);
        void EnableLayer(const char* layer);

    protected:
        void CreateQueueIndices();
        void CreateLogicalDevice();
        void AddEnabledFeatures();
        void AddEnabledLayersAndExtensions();
        
    protected:
        SharedPtr<VulkanInstance> mVkInstance;
        SharedPtr<VulkanPhysicalDevice> mPhysicalDevice;
        SharedPtr<VulkanSurface> mSurface;

        VkDevice mLogicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceFeatures mEnabledFeatures = {};
        PCCHArray mEnabledLayers;
        PCCHArray mEnabledExtensions;

        VkQueueFlags mSupportedQueues = {};
        Uint mGraphicsFamily = 0;
        Uint mPresentFamily = 0;
        Uint mComputeFamily = 0;
        Uint mTransferFamily = 0;

        VkQueue mGraphicsQueue = VK_NULL_HANDLE;
        VkQueue mPresentQueue = VK_NULL_HANDLE;
        VkQueue mComputeQueue = VK_NULL_HANDLE;
        VkQueue mTransferQueue = VK_NULL_HANDLE;
    };
}