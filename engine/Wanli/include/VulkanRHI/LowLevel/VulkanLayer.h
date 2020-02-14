#pragma once

#include "VulkanRHI/VulkanConfig.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    class DLLDECL VulkanLayer
    {
    public:
        inline const VkLayerProperties& GetProperties() const { return mLayer; };
        inline const Array<VkExtensionProperties>& GetRequiredExtensions() const { return mRequiredExtensions; };

    protected:
        VulkanLayer() = default;

    protected:
        VkLayerProperties mLayer = {};
        Array<VkExtensionProperties> mRequiredExtensions;
    };

    class DLLDECL VulkanInstanceLayer final : public VulkanLayer
    {
    public:
        VulkanInstanceLayer(const char* layerName);
    };

    class DLLDECL VulkanDeviceLayer : public VulkanLayer
    {
    public:
        VulkanDeviceLayer(const char* layerName, VkPhysicalDevice device);
    };
}