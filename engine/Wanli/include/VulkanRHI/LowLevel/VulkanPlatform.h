#pragma once

#include "VulkanRHI/VulkanConfig.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    class DLLDECL VulkanPlatform
    {
    public:
        static void LoadVulkanLibrary();
        static void FreeVulkanLibrary();
  
        static void GetInstanceExtensions(PCCHArray& extensions);
        static void GetDeviceExtensions(PCCHArray& extensions);

        static VkSurfaceKHR CreateSurface(VkInstance instance);
    };
}