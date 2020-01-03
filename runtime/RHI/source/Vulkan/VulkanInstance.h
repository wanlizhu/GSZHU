#pragma once

#include "Wanlix/VulkanRHI/Common.h"

namespace Wanlix
{
    class VulkanInstance final : public Object
    {
        CLASSINFO(VulkanInstance)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        static SharedPtr<VulkanInstance> Create(Bool enableValidation,
                                                Uint globalExtensionCount,
                                                StringArray globalExtensionNames,
                                                VkAllocationCallbacks* allocator);
        ~VulkanInstance();

        Bool IsLayerSupported(const char* name) const;
        Bool IsExtensionSupported(const char* name) const;
        VkPhysicalDevice SelectPhysicalDevice() const;
        VkAllocationCallbacks* GetVkAllocator() const;
        VkInstance GetVkInstance() const;

    private:
        VulkanInstance(Bool enableValidation,
                       Uint globalExtensionCount,
                       StringArray globalExtensionNames,
                       VkAllocationCallbacks* allocator);

    private:
        Bool mDebugEnabled = false;
        VkAllocationCallbacks* mVkAllocator = nullptr;
        VkInstance mVkInstance = VK_NULL_HANDLE;

        Array<VkLayerProperties> mLayers;
        Array<VkExtensionProperties> mExtensions;
        Array<VkPhysicalDevice> mPhysicalDevices;
    };
}