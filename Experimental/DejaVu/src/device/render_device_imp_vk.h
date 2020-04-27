#pragma once

#include "render_device.h"
#include "vulkan/vulkan_handle.h"
#include "vulkan/vulkan_swapchain.h"
#include <vulkan/vulkan.h>

namespace djv
{
    class RenderDevice_Vulkan : public IRenderDevice
    {
    public:
        RenderDevice_Vulkan(
            const std::vector<const char>& validationLayers,
            const std::vector<const char>& instanceExtensions,
            const std::vector<const char>& deviceExtensions
        );
        RenderDevice_Vulkan(const RenderDevice_Vulkan&) = delete;
        RenderDevice_Vulkan(RenderDevice_Vulkan&& rhs) = delete;
        virtual ~RenderDevice_Vulkan();

        RenderDevice_Vulkan& operator=(const RenderDevice_Vulkan&) = delete;
        RenderDevice_Vulkan& operator=(RenderDevice_Vulkan&& rhs) = delete;
        
        VkPhysicalDevice getPhysicalDevice() const;
        VkDevice getLogicalDevice() const;
        VkQueue  getQueue(uint32_t familyIndex) const;
        uint32_t getQueueFamilyIndex(VkQueueFlagBits queueFlags) const;
        uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlagBits props) const;
        bool isValid() const;

        virtual void waitIdle() const override;

    protected:
        bool setupLogicalDevice(
            VkPhysicalDevice physicalDevice, 
            const VkPhysicalDeviceFeatures& enabledFeatures, 
            const std::vector<const char*>& enabledExtensions, 
            VkQueueFlags requestedQueueTypes,
            bool useSwapchain
        );
        bool setupPresentQueue(VkSurfaceKHR surface);
        
    private:
        using QueueIndex = uint32_t;

        VulkanHandle<VkInstance> mVulkanInstance;
        VulkanHandle<VkPhysicalDevice> mPhysicalDevice;
        VulkanHandle<VkDevice> mLogicalDevice;

        std::unordered_map<QueueIndex, VkQueueFlagBits> mQueueFlagMap;
        std::unordered_map<QueueIndex, VulkanHandle<VkQueue>> mQueueHandleMap;
        std::unordered_map<QueueIndex, VulkanHandle<VkCommandPool>> mCommandPoolMap;

        std::shared_ptr<VulkanSwapchain> mSwapchain;
        std::shared_ptr<VulkanImage> mDepthStencilTarget;
    };
}