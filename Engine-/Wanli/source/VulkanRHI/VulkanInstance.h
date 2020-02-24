#pragma once

#include "VulkanConfig.h"

#define LOAD_FUNC(name) (PFN_##name)VulkanInstance::LoadFunction(#name)
#define MUST_LOAD_FUNC(name) (PFN_##name)VulkanInstance::LoadFunction2(#name)

namespace Wanli
{
    class VulkanPhysicalDevice;

    class VulkanInstance final
    {
    public:
        static VkInstance Instance;

        static void Create();
        static void Destroy();
        static void* LoadFunction(const char* name);
        static void* LoadFunction2(const char* name);
        static std::vector<VulkanPhysicalDevice> GetPhysicalDevices();

    private:
        static void PrepareDebugCallback(VkInstanceCreateInfo& instanceInfo);
        static void CreateDebugCallback();
        static void DestroyDebugCallback();
    };
}