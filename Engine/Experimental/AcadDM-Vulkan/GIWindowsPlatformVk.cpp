#ifdef _WIN32

#include "GIWindowsPlatformVk.h"
#include "GIPlatformVk.h"
#include <Windows.h>
#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    void* GIWindowsPlatformVk::libvulkan = nullptr;

    bool GIWindowsPlatformVk::LoadVulkanLibrary()
    {
        libvulkan = ::LoadLibrary(TEXT("vulkan-1.dll"));
        return libvulkan != nullptr;
    }

    void GIWindowsPlatformVk::LoadVulkanFunctions()
    {}

    void GIWindowsPlatformVk::FreeVulkanLibrary()
    {
        if (libvulkan)
        {
            ::FreeLibrary((HMODULE)libvulkan);
            libvulkan = nullptr;
        }
    }

    void GIWindowsPlatformVk::RequiresInstanceExtensions(std::vector<const char*>& extensions)
    {
        extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    }

    void GIWindowsPlatformVk::RequiresDeviceExtensions(EGPUVendorID vendorId, std::vector<const char*>& extensions)
    {
        extensions.push_back(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME);

        if (vendorId == EGPUVendorID::AMD)
        {
            extensions.push_back(VK_AMD_BUFFER_MARKER_EXTENSION_NAME);
        }
        if (vendorId == EGPUVendorID::Nvidia)
        {
            extensions.push_back(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
        }
    }

    void GIWindowsPlatformVk::RequiresDeviceFeatures(VkPhysicalDeviceFeatures& features)
    {

    }

    void GIWindowsPlatformVk::CreateSurface(VkInstance instance, void* window, VkSurfaceKHR* surface)
    {
        VkWin32SurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.hinstance = GetModuleHandle(nullptr);
        createInfo.hwnd = (HWND)window;
        VK_CHECK(vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, surface));
    }
}

#endif