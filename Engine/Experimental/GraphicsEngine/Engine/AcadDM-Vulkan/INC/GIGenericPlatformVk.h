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
        static bool LoadVulkanLibrary() { return true; }
        static void LoadVulkanFunctions() {}
        static void FreeVulkanLibrary() {}

        static std::vector<const char*> const& RequiredInstanceLayers() {}
        static std::vector<const char*> const& RequiredInstanceExtensions() {}
        static std::vector<const char*> const& RequiredDeviceExtensions(EGPUVendorID vendorId) {}
        static VkPhysicalDeviceFeatures const& RequiredDeviceFeatures() {}
        
        static void CreateSurface(VkInstance instance, void* window, VkSurfaceKHR* surface) {}
        static void PrePresent(VkQueue queue, VkPresentInfoKHR& presentInfo) {}
        
        static uint32_t FindFiles(
            const std::filesystem::path& root,
            const std::filesystem::path& filename,
            bool recursive,
            bool findFirst,
            std::vector<std::filesystem::path>& results)
        {
            return 0;
        }
    };
}