#ifdef _WIN32

#include "GIWindowsPlatformVk.h"
#include "GIPlatformVk.h"
#include <Windows.h>
#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIWindowsPlatformVk& GIWindowsPlatformVk::Get()
    {
        static GIWindowsPlatformVk instance;
        return instance;
    }

    bool GIWindowsPlatformVk::LoadVulkanLibrary()
    {
        m_libvulkan = ::LoadLibrary(TEXT("vulkan-1.dll"));
        return m_libvulkan != nullptr;
    }

    void GIWindowsPlatformVk::LoadVulkanFunctions()
    {}

    void GIWindowsPlatformVk::FreeVulkanLibrary()
    {
        if (m_libvulkan)
        {
            ::FreeLibrary((HMODULE)m_libvulkan);
            m_libvulkan = nullptr;
        }
    }

    std::vector<const char*> GIWindowsPlatformVk::RequiredInstanceLayers()
    {
        static std::vector<const char*> layers;
        if (layers.empty())
        {
            /*
             * Use of the meta-layer is no longer recommended,
             * please replace references to VK_LAYER_LUNARG_standard_validation with VK_LAYER_KHRONOS_validation
            */
            layers.push_back("VK_LAYER_KHRONOS_validation");
        }
        return layers;
    }

    std::vector<const char*> GIWindowsPlatformVk::RequiredInstanceExtensions()
    {
        static std::vector<const char*> extensions;
        if (extensions.empty())
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        }
        return extensions;
    }

    std::vector<const char*> GIWindowsPlatformVk::RequiredDeviceExtensions(EGPUVendorID vendorId)
    {
        static std::vector<const char*> extensions;
        if (extensions.empty())
        {
            extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
            extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

            if (vendorId == EGPUVendorID::AMD)
            {
                extensions.push_back(VK_AMD_BUFFER_MARKER_EXTENSION_NAME);
            }
            if (vendorId == EGPUVendorID::Nvidia)
            {
                extensions.push_back(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
            }
        }
        return extensions;
    }

    VkPhysicalDeviceFeatures GIWindowsPlatformVk::RequiredDeviceFeatures()
    {
        static VkPhysicalDeviceFeatures features = {};
        return features;
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
    /*
    uint32_t GIWindowsPlatformVk::FindFiles(
        const std::filesystem::path& root,
        const std::filesystem::path& filename,
        bool recursive,
        bool findFirst,
        std::vector<std::filesystem::path>& results)
    {
        std::wstring searchPath = (root / filename).wstring();
        WIN32_FIND_DATAW findData = {};
        HANDLE findHandle = ::FindFirstFileW(searchPath.c_str(), &findData);

        if (findHandle != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (wcscmp(findData.cFileName, L".") || wcscmp(findData.cFileName, L".."))
                    continue;

                if (findFirst && !results.empty())
                    break;

                results.emplace_back(root / findData.cFileName);

                if (recursive && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    auto subdir = root / findData.cFileName;
                    std::vector<std::filesystem::path> subset;
                    if(FindFiles(subdir, filename, recursive, findFirst, subset))
                        results.insert(results.end(), subset.begin(), subset.end());
                }
            } while (::FindNextFileW(findHandle, &findData));

            ::FindClose(findHandle);
        }

        return (uint32_t)results.size();
    }*/
}

#endif