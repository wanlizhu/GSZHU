#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIInstanceVk;
    class GIDeviceVk;

    class GIDeviceBuilderVk
    {
    public:
        GIDeviceBuilderVk(SharedPtr<GIInstanceVk> instance);

        GIDeviceBuilderVk& SetPhysicalDevice(VkPhysicalDevice physicalDevice);
        GIDeviceBuilderVk& EnableExtension(const char* name, bool compulsory = false);
        GIDeviceBuilderVk& EnableExtensions(const std::vector<const char*>& names);
        GIDeviceBuilderVk& EnableFeatures(const VkPhysicalDeviceFeatures& features);
        GIDeviceBuilderVk& CreateGraphicsQueue();
        GIDeviceBuilderVk& CreateComputeQueue();
        GIDeviceBuilderVk& CreateTransferQueue();

        SharedPtr<GIDeviceVk> Build();

    private:
        bool FindFromQueueCreateInfos(uint32_t familyIndex);

    private:
        SharedPtr<GIInstanceVk> mInstance;
        VkPhysicalDevice mPhysicalDeviceHandle = VK_NULL_HANDLE;
        VkDeviceCreateInfo mCreateInfo = {};

        VkPhysicalDeviceFeatures mSupportedFeatures = {};
        std::vector<VkExtensionProperties> mSupportedExtensions;
        std::vector<VkQueueFamilyProperties> mSupportedQueueFamilies;

        VkPhysicalDeviceFeatures mEnabledFeatures = {};
        std::vector<const char*> mEnabledExtensions;
        std::vector<VkDeviceQueueCreateInfo> mQueueCreateInfos;
    };
}