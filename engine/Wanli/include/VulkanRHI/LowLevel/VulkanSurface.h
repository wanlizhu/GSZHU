#pragma once

#include "VulkanRHI/VulkanConfig.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    class VulkanInstance;
    class VulkanPhysicalDevice;

    class DLLDECL VulkanSurface : public NonCopyable
        , public std::enable_shared_from_this<VulkanSurface>
    {
    public:
        VulkanSurface(VulkanInstance* instance,
                      VulkanPhysicalDevice* physicalDevice);
        virtual ~VulkanSurface();

        virtual bool Initialize();
        virtual void Destroy();

        inline operator const VkSurfaceKHR&() const { return mSurface; }
        inline const VkSurfaceKHR& GetHandle() const { return mSurface; }
        inline const VkSurfaceCapabilitiesKHR& GetCapabilities() const { return mCapabilities; }
        inline const VkSurfaceFormatKHR& GetFormat() const { return mFormat; }

    protected:
        SharedPtr<VulkanInstance> mVkInstance;
        SharedPtr<VulkanPhysicalDevice> mPhysicalDevice;

        VkSurfaceKHR mSurface = VK_NULL_HANDLE;
        VkSurfaceCapabilitiesKHR mCapabilities = {};
        VkSurfaceFormatKHR mFormat = {};
    };
}