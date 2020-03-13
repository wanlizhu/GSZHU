#pragma once

#include <stdint.h>
#include <vector>
#include <vulkan/vulkan.h>

namespace AutoCAD::Graphics::Engine
{
    enum class EGPUVendorID
    {
        Unknown = -1,
        NotQueried = 0,
        AMD = 0x1002,
        ImgTec = 0x1010,
        Nvidia = 0x10DE,
        ARM = 0x13B5,
        Qualcomm = 0x5143,
        Intel = 0x8086,
    };

    class GIGenericPlatformVk
    {
    public:
        static bool LoadVulkanLibrary() { return true; }
        static void LoadVulkanFunctions() {}
        static void FreeVulkanLibrary() {}

        static void RequiresInstanceExtensions(std::vector<const char*>& extensions) {}
        static void RequiresDeviceExtensions(EGPUVendorID vendorId, std::vector<const char*>& extensions) {}
        static void RequiresDeviceFeatures(VkPhysicalDeviceFeatures& features) {}
        
        static void CreateSurface(VkInstance instance, void* window, VkSurfaceKHR* surface) {}
        static void PrePresent(VkQueue queue, VkPresentInfoKHR& presentInfo) {}
    };
}