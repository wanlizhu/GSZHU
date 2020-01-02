#pragma once

#include "Wanlix/VulkanRHI/Common.h"

namespace Wanlix
{
    class PhysicalDevice final : public Object
    {
        CLASSINFO(PhysicalDevice)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        using UniquePtr = std::unique_ptr<PhysicalDevice>;

        static UniquePtr Create(VkPhysicalDevice device);

        Uint32 FindQueueFamily(VkQueueFlags flags) const;
        VkPhysicalDevice GetVkPhysicalDevice() const;
        Bool IsExtensionSupported(const char* name) const;
        Bool IsPresentSupported(Uint32 queueFamily, VkSurfaceKHR surface) const;

        Uint32 GetMemoryTypeIndex(Uint32 type, VkMemoryPropertyFlags properties) const;
        VkPhysicalDeviceProperties const& GetProperties() const;

    private:
        PhysicalDevice(VkPhysicalDevice device);

    private:
        const VkPhysicalDevice mVkDevice;
        VkPhysicalDeviceProperties mProperties = {};
        VkPhysicalDeviceFeatures mFeatures = {};
        VkPhysicalDeviceMemoryProperties mMemoryProperties = {};
        Array<VkQueueFamilyProperties> mQueueFamilyProperties;
        Array<VkExtensionProperties> mSupportedExtensions;
    };
}