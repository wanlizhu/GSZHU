#pragma once

#include "VulkanConfig.h"

#define LOAD_FUNC(name) (PFN_##name)vkGetInstanceProcAddr(VulkanInstance::globalInstance, #name)

namespace Wanli
{
    struct VulkanPhysicalDevice;

    struct VulkanInstance
    {
        static VkInstance globalInstance;
        VkInstance instance = VK_NULL_HANDLE;

        VulkanInstance();
        ~VulkanInstance();

        inline operator const VkInstance& () const { return instance; }
        void* LoadFunction(const char* name);
        void* LoadFunction2(const char* name);
        std::vector<VulkanPhysicalDevice> GetPhysicalDevices();
    };
}