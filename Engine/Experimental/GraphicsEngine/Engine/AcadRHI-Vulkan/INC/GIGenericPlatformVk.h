#pragma once

#include <stdint.h>
#include <vector>
#include <filesystem>
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
        bool LoadVulkanLibrary() { return true; }
        void LoadVulkanFunctions() {}
        void FreeVulkanLibrary() {}

        std::vector<const char*> RequiredInstanceLayers() { return {}; }
        std::vector<const char*> RequiredInstanceExtensions() { return {}; }
        std::vector<const char*> RequiredDeviceExtensions(EGPUVendorID vendorId) { return {}; }
        VkPhysicalDeviceFeatures RequiredDeviceFeatures() { return {}; }
        
        void CreateSurface(VkInstance instance, void* window, VkSurfaceKHR* surface) {}
        void PrePresent(VkQueue queue, VkPresentInfoKHR& presentInfo) {}
    };
}