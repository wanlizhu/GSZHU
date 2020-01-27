#pragma once

#include "Wanlix/Common.h"
#include <vulkan/vulkan.h>

namespace Wanlix
{
    class VulkanInstance final : public std::enable_shared_from_this<VulkanInstance>
    {
    public:
        static SharedPtr<VulkanInstance> Create(Bool enableValidation,
                                                StringArray globalExtensions,
                                                VkAllocationCallbacks* allocator);
        ~VulkanInstance();

        inline SharedPtr<VulkanInstance> GetSharedPtr() { return shared_from_this(); }
        inline VkInstance GetVkInstance() const { return mVkInstance; }
        inline VkAllocationCallbacks* GetAllocator() const { return mVkAllocator; }
        Bool IsLayerAvailable(const String& name) const;
        Bool IsExtensionAvailable(const String& name) const;
        VkPhysicalDevice SelectPhysicalDevice() const;

    private:
        VulkanInstance(Bool enableValidation,
                       StringArray globalExtensions,
                       VkAllocationCallbacks* allocator);
        VulkanInstance(const VulkanInstance&) = delete;
        VulkanInstance(VulkanInstance&&) = delete;
        VulkanInstance& operator = (const VulkanInstance&) = delete;
        VulkanInstance& operator = (VulkanInstance&&) = delete;

    private:
        VkInstance mVkInstance = VK_NULL_HANDLE;
        Bool mDebugEnabled = false;
        VkAllocationCallbacks* mVkAllocator = nullptr;
        Array<VkLayerProperties> mLayers;
        Array<VkExtensionProperties> mExtensions;
        Array<VkPhysicalDevice> mPhysicalDevices;
    };

    class PhysicalDevice final
    {
    public:
        static SharedPtr<PhysicalDevice> Create(VkPhysicalDevice device);
        ~PhysicalDevice();

        inline VkPhysicalDevice GetVkPhysicalDevice() const { return mVkPhysicalDevice; }
        inline const VkPhysicalDeviceProperties& GetProperties() const { return mProperties; }
        Uint FindQueueFamily(VkQueueFlags flags) const;
        Bool IsExtensionSupported(const String& name) const;
        Bool IsPresentSupported(Uint queueFamily, VkSurfaceKHR surface) const;
        Uint GetMemoryTypeIndex(Uint type, VkMemoryPropertyFlags flags) const;

    private:
        PhysicalDevice(VkPhysicalDevice device);
        PhysicalDevice(const PhysicalDevice&) = delete;
        PhysicalDevice(PhysicalDevice&&) = delete;
        PhysicalDevice& operator = (const PhysicalDevice&) = delete;
        PhysicalDevice& operator = (PhysicalDevice&&) = delete;

    private:
        VkPhysicalDevice mVkPhysicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties mProperties = {};
        VkPhysicalDeviceFeatures mFeatures = {};
        VkPhysicalDeviceMemoryProperties mMemoryProperties = {};
        Array<VkQueueFamilyProperties> mQueueFamilyProperties;
        Array<VkExtensionProperties> mSupportedExtensions;
    };

    class LogicalDevice final : public std::enable_shared_from_this<LogicalDevice>
    {
    public:
        static SharedPtr<LogicalDevice> Create(VkPhysicalDevice physicalDevice, 
                                               const VkDeviceCreateInfo& info,
                                               VkAllocationCallbacks* allocator);
        ~LogicalDevice();

        inline SharedPtr<LogicalDevice> GetSharedPtr() { return shared_from_this(); }
        inline VkDevice GetVkDevice() const { return mVkDevice; }
        VkQueue GetQueue(Uint queueFamily, Uint queueIndex) const;
        void WaitIdle() const;

    private:
        LogicalDevice(VkPhysicalDevice physicalDevice,
                      const VkDeviceCreateInfo& info,
                      VkAllocationCallbacks* allocator);
        LogicalDevice(const LogicalDevice&) = delete;
        LogicalDevice(LogicalDevice&&) = delete;
        LogicalDevice& operator = (const LogicalDevice&) = delete;
        LogicalDevice& operator = (LogicalDevice&&) = delete;

    private:
        VkDevice mVkDevice = VK_NULL_HANDLE;
        VkAllocationCallbacks* mAllocator = nullptr;
        VkPipelineStageFlags mEnabledShaderStages = 0;
    };
}