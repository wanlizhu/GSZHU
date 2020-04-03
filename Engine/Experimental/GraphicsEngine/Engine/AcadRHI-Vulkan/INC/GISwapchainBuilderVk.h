#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;
    class GISwapchainVk;

    class GISwapchainBuilderVk
    {
    public:
        GISwapchainBuilderVk(SharedPtr<GIDeviceVk> device);

        GISwapchainBuilderVk& SetWindow(void* window);
        GISwapchainBuilderVk& SetExtent(VkExtent2D extent, bool compulsory = false);
        GISwapchainBuilderVk& AcquireNextImageAsync(bool value);
        GISwapchainBuilderVk& EnableVsync(bool value);
        GISwapchainBuilderVk& SetBackbufferCount(uint32_t count);
        GISwapchainBuilderVk& SetColorFormat(VkFormat format, bool compulsory = false);
        GISwapchainBuilderVk& SetPreTransform(VkSurfaceTransformFlagBitsKHR preTransform);
        GISwapchainBuilderVk& SetComponentMapping(VkComponentMapping componentMapping);
        GISwapchainBuilderVk& SetOldSwapchain(VkSwapchainKHR oldSwapchain);

        SharedPtr<GISwapchainVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        bool mAcquireNextImageAsync = true;
        VkComponentMapping mComponentMapping;
        VkSwapchainCreateInfoKHR mCreateInfo;
    };
}