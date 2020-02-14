#include "VulkanRHI/LowLevel/VulkanPlatform.h"
#include "Engine.h"
#include "VulkanRHI/LowLevel/VulkanDebug.h"

namespace Wanli
{
    void VulkanPlatform::LoadVulkanLibrary()
    {}

    void VulkanPlatform::FreeVulkanLibrary()
    {}
    
    void VulkanPlatform::GetInstanceExtensions(PCCHArray& extensions)
    {
        extensions = {};
    }

    void VulkanPlatform::GetDeviceExtensions(PCCHArray& extensions)
    {}
    
    VkSurfaceKHR VulkanPlatform::CreateSurface(VkInstance instance)
    {
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
        surfaceCreateInfo.hwnd = (HWND)Engine::Get()->GetWindowHandle();
        VkResult error = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);
        CheckVkResult(error);
        
        return surface;
    }
}