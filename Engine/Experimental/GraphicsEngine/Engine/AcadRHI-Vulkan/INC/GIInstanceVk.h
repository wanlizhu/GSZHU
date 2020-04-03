#pragma once

#include "GIPlatformVk.h"
#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIInstanceVk
        : public GINonCopyable
        , public std::enable_shared_from_this<GIInstanceVk>
    {
        friend class GIDeviceBuilderVk;
        friend class GIInstanceBuilderVk;
    public:
        static const VkAllocationCallbacks* GetHostAlloc();
        static bool IsLayerSupported(const char* name);
        static bool IsExtensionSupported(const char* name);

        ~GIInstanceVk();
        operator const VkInstance& () const;
        bool IsValid() const;
        bool IsValidationLayerEnabled() const;
        bool IsLayerEnabled(const char* name) const;
        bool IsExtensionEnabled(const char* name) const;

        uint32_t GetPhysicalDeviceCount() const;
        VkPhysicalDevice FirstPhysicalDevice() const;
        VkPhysicalDevice FindPhysicalDeviceBy(
            const std::vector<const char*>& extensions,
            const std::vector<const char*>& extensionsOptional,
            VkQueueFlags queueFlags,
            const VkPhysicalDeviceFeatures& features
        );
        
        VkPhysicalDevice GetChosenGPU() const;
        EGPUVendorID GetChosenGPUVendorID() const;

    protected:
        GIInstanceVk(
            const VkInstanceCreateInfo& createInfo,
            const std::vector<const char*>& enabledLayers,
            const std::vector<const char*>& enabledExtensions
        );

    private:
        static inline VkAllocationCallbacks mAllocationCallbacks = {};

        VkInstance mInstanceHandle = VK_NULL_HANDLE;
        VkPhysicalDevice mChosenPhysicalDeviceHandle = VK_NULL_HANDLE;
        std::vector<const char*> mEnabledLayers; // referenced by vkCreateDevice(...)
        std::vector<const char*> mEnabledExtensions;
        VkDebugUtilsMessengerEXT mDebugUtilsHandle = VK_NULL_HANDLE;
    };
}