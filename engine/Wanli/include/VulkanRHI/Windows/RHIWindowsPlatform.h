#pragma once

#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.h>
#include "VulkanRHI/RHIPlatform.h"

namespace Wanli
{
    class DLLDECL RHIWindowsPlatform : public RHIPlatform
    {
    public:
        virtual bool LoadVulkanLibrary() override final;
        virtual void FreeVulkanLibrary() override final;
        virtual bool LoadVulkanInstanceFunctions(VkInstance instance) override final;
        virtual bool CreateSurface(VkInstance instance, VkSurfaceKHR& surface) override final;

        virtual void GetInstanceExtensions(PCCHArray& extensions) override final;
        virtual void GetDeviceExtensions(PCCHArray& extensions) override final;
    };
}