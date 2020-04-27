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
        static GIWindowsPlatformVk& Get();

        bool LoadVulkanLibrary();
        void LoadVulkanFunctions();
        void FreeVulkanLibrary();

        std::vector<const char*> RequiredInstanceLayers();
        std::vector<const char*> RequiredInstanceExtensions();
        std::vector<const char*> RequiredDeviceExtensions(EGPUVendorID vendorId);
        VkPhysicalDeviceFeatures RequiredDeviceFeatures();

        void CreateSurface(VkInstance instance, void* window, VkSurfaceKHR* surface);
        void PrePresent(VkQueue queue, VkPresentInfoKHR& presentInfo) {}

    private:
        void* m_libvulkan;
    };

    using GIPlatformVk = GIWindowsPlatformVk;
}

#endif