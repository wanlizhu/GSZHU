#pragma once

#include "render_device.h"
#include "vulkan/vulkan_handle.h"
#include "vulkan/vulkan_swapchain.h"
#include <vulkan/vulkan.h>

namespace djv
{
    enum class QueueFamily
    {
        Graphics,
        Transfer,
        Compute,
        Present,
    };

    class RenderDevice_Vulkan : public IRenderDevice
    {
    public:
        RenderDevice_Vulkan(
            const std::vector<const char>& validationLayers,
            const std::vector<const char>& instanceExtensions,
            const std::vector<const char>& deviceExtensions
        );
        RenderDevice_Vulkan(const RenderDevice_Vulkan&) = delete;
        RenderDevice_Vulkan(RenderDevice_Vulkan&& rhs);
        virtual ~RenderDevice_Vulkan();

        RenderDevice_Vulkan& operator=(const RenderDevice_Vulkan&) = delete;
        RenderDevice_Vulkan& operator=(RenderDevice_Vulkan&& rhs);
        
        VkPhysicalDevice getPhysicalDevice() const;
        VkDevice getLogicalDevice() const;
        VkQueue  getQueue(uint32_t family) const;
        uint32_t getQueueFamilyIndex(QueueFamily family) const;
        uint32_t getMemoryTypeIndex(VkMemoryPropertyFlagBits props) const;
        bool isValid() const;

        virtual void waitIdle() const override;
        virtual bool setupSwapchain(void* window, int imageCount) override;

    protected:
        bool setupPresentQueue(VkSurfaceKHR surface);
        
    private:
        using QueueIndex = uint32_t;

        VulkanHandle<VkInstance> mVulkanInstance;
        VulkanHandle<VkPhysicalDevice> mPhysicalDevice;
        VulkanHandle<VkDevice> mLogicalDevice;

        std::optional<QueueIndex> mGraphicsQueueFamily;
        std::optional<QueueIndex> mTransferQueueFamily;
        std::optional<QueueIndex> mComputeQueueFamily;
        std::optional<QueueIndex> mPresentQueueFamily;

        std::unordered_map<QueueIndex, VulkanHandle<VkQueue>> mQueueHandleMap;
        std::unordered_map<QueueIndex, VulkanHandle<VkCommandPool>> mCommandPoolMap;

        std::shared_ptr<VulkanSwapchain> mSwapchain;
        std::shared_ptr<VulkanImage> mDepthStencilTarget;
    };
}