#pragma once

#include "VulkanRHI/VulkanConfig.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    class VulkanInstance;

    class DLLDECL VulkanPhysicalDevice : public NonCopyable
        , public std::enable_shared_from_this<VulkanPhysicalDevice>
    {
    public:
        explicit VulkanPhysicalDevice(VulkanInstance* instance);
        virtual ~VulkanPhysicalDevice();

        virtual bool Initialize();
        virtual void Destroy();

        inline operator const VkPhysicalDevice& () const { return mPhysicalDevice; }
        inline const VkPhysicalDevice& GetHandle() const { return mPhysicalDevice; }
        inline const VkPhysicalDeviceProperties& GetProperties() const { return mProperties; }
        inline const VkPhysicalDeviceFeatures& GetSupportedFeatures() const { return mSupportedFeatures; }
        inline const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const { return mMemoryProperties; }
        inline VkSampleCountFlagBits GetMSAASamples() const { return mMSAASamples; }
        inline const PCCHArray& GetSupportedExtensions() const { return mSupportedExtensions; }
        inline const PCCHArray& GetSupportedLayers() const { return mSupportedLayers; }

    protected:
        int RankPhysicalDevice(VkPhysicalDevice device);
        void ChoosePhysicalDevice();
        void LoadSupportedLayersAndExtensions();
        void LoadMaxSampleCount();
        void LogPhysicalDevice(
            const VkPhysicalDeviceProperties& properties,
            const Array<VkExtensionProperties>& extensions);

    protected:
        SharedPtr<VulkanInstance> mVkInstance;
        VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties mProperties = {};
        VkPhysicalDeviceFeatures mSupportedFeatures = {};
        VkPhysicalDeviceMemoryProperties mMemoryProperties = {};
        VkSampleCountFlagBits mMSAASamples = VK_SAMPLE_COUNT_1_BIT;

        PCCHArray mSupportedExtensions;
        PCCHArray mSupportedLayers;
    };
}