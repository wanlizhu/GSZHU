#pragma once

#include "VulkanConfig.h"
#include "VulkanError.h"

namespace Wanli
{
    class VulkanInstance;
    class VulkanPhysicalDevice;
    class VulkanLogicalDevice;
    class VulkanSurface;

    class DLLDECL VulkanRHI : public NonCopyable
        , public std::enable_shared_from_this<VulkanRHI>
    {
    public:
        VulkanRHI(WindowHandle window);
        virtual ~VulkanRHI() = default;



    private:
        UniquePtr<VulkanInstance> mInstance;
        UniquePtr<VulkanPhysicalDevice> mPhysicalDevice;
        UniquePtr<VulkanLogicalDevice> mLogicalDevice;
        UniquePtr<VulkanSurface> mSurface;
    };
}