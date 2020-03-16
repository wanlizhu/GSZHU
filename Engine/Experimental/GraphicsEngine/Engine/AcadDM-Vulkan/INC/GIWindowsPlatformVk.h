#pragma once
#if defined(_WIN32)

#define VK_USE_PLATFORM_WIN32_KHR 1
#define VK_USE_PLATFORM_WIN32_KHX 1
#include "GIGenericPlatformVk.h"

#ifdef _DEBUG
#define LOG_DEBUG(...) fprintf(stdout, __VA_ARGS__)
#define LOG_WARNING(...) fprintf(stdout, __VA_ARGS__)
#define LOG_ERROR(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#define LOG_WARNING(...)
#define LOG_ERROR(...)
#endif

namespace AutoCAD::Graphics::Engine
{
    class GIWindowsPlatformVk : public GIGenericPlatformVk
    {
    public:
        static bool LoadVulkanLibrary();
        static void LoadVulkanFunctions();
        static void FreeVulkanLibrary();

        static std::vector<const char*> const& RequiredInstanceLayers();
        static std::vector<const char*> const& RequiredInstanceExtensions();
        static std::vector<const char*> const& RequiredDeviceExtensions(EGPUVendorID vendorId);
        static VkPhysicalDeviceFeatures const& RequiredDeviceFeatures();

        static void CreateSurface(VkInstance instance, void* window, VkSurfaceKHR* surface);
        static void PrePresent(VkQueue queue, VkPresentInfoKHR& presentInfo) {}

    private:
        static void* libvulkan;
    };

    using GIPlatformVk = GIWindowsPlatformVk;
}

#endif