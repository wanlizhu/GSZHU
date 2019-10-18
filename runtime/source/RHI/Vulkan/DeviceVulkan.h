#pragma once

#include "GE2/RHI/IDevice.h"
#include <vulkan/vulkan.h>

namespace GE2::RHI
{
    class DeviceVulkan final : public IDevice
    {
    public:
        DeviceVulkan() = default;
        virtual ~DeviceVulkan() override;
        virtual bool Initialize(const InitializeData& data) override;
        virtual void Destroy() override;

    private:
        void CreateInstance(const STRLIST& extensions);
        void EnableSpecificLayers(const STRLIST& layers);
        void EnumeratePhysicalDevices(const STRLIST& extensions);
        void CreateLogicalDevices();

    private:
        VkInstance mInstance;
        std::unordered_map<std::string, bool> mInstanceExtensionMap;
        std::vector<std::string>              mEnabledLayerList;

        std::vector<VkPhysicalDevice>         mPhysicalDevices;
        std::unordered_map<std::string, bool> mDeviceExtensionMap;
        std::unordered_map<std::string, bool> mSupportedFeatureMap;
        
        uint32_t mPhysicalDeviceIndex = 0;
        VkDevice mDevice;
    };
}