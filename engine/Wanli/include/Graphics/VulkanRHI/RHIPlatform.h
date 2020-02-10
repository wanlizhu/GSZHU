#pragma once

#include "BasicTypes.h"

namespace Wanli
{
    class DLLDECL RHIPlatform
    {
    public:
        static RHIPlatform* Get();

        virtual bool LoadVulkanLibrary() = 0;
        virtual void FreeVulkanLibrary() = 0;
        virtual bool LoadVulkanInstanceFunctions(VkInstance instance) = 0;
        virtual bool CreateSurface(VkInstance instance, VkSurfaceKHR& surface) = 0;

        virtual void GetInstanceExtensions(PCCHArray& extensions) = 0;
        virtual void GetDeviceExtensions(PCCHArray& extensions) = 0;
    };
}