#include "Graphics/VulkanRHI/Windows/RHIWindowsPlatform.h"
#include "Engine.h"
#include <Windows.h>

namespace Wanli
{
    static PFN_vkGetInstanceProcAddr gp_vkGetInstanceProcAddr = nullptr;

    RHIPlatform* RHIPlatform::Get()
    {
        static RHIWindowsPlatform instance;
        return &instance;
    }

    bool RHIWindowsPlatform::LoadVulkanLibrary()
    {
        return true;
    }

    void RHIWindowsPlatform::FreeVulkanLibrary()
    {}

    bool RHIWindowsPlatform::LoadVulkanInstanceFunctions(VkInstance instance)
    {
        gp_vkGetInstanceProcAddr = vkGetInstanceProcAddr;
        return true;
    }

    bool RHIWindowsPlatform::CreateSurface(VkInstance instance, VkSurfaceKHR& surface)
    {
        return Engine::Get()->GetApplication()->GetWindow()->CreateVkSurface(instance, surface);
    }

    void RHIWindowsPlatform::GetInstanceExtensions(PCCHArray& extensions)
    {
        Engine::Get()->GetApplication()->GetWindow()->GetRequiredExtensions(extensions);
    }

    void RHIWindowsPlatform::GetDeviceExtensions(PCCHArray& extensions)
    {}
}