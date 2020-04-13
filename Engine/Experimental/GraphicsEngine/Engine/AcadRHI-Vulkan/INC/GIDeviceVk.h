#pragma once

#include "GIPlatformVk.h"
#include "GICommonVk.h"
#include "GIInstanceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GICommandQueueVk;

    class GIDeviceVk : public std::enable_shared_from_this<GIDeviceVk>
    {
    public:
        static SharedPtr<GIDeviceVk> Create(
            SharedPtr<GIInstanceVk> instance,
            VkPhysicalDevice physicalDevice
        );

        virtual ~GIDeviceVk();
        operator const VkDevice& () const;
        bool IsValid() const;
        bool IsExtensionEnabled(const char* name) const;
        void SetupPresentQueue(VkSurfaceKHR surface);
        
        SharedPtr<GIInstanceVk> const& GetInstance() const;
        uint32_t GetMemoryTypeIndex(uint32_t typeFilters, VkMemoryPropertyFlags properties) const;
        VkPhysicalDevice GetPhysicalDevice() const;
        VkDevice GetLogicalDevice() const;
        SharedPtr<GICommandQueueVk> GetGraphicsQueue() const;
        SharedPtr<GICommandQueueVk> GetComputeQueue() const;
        SharedPtr<GICommandQueueVk> GetTransferQueue() const;
        SharedPtr<GICommandQueueVk> GetPresentQueue() const;

        void WaitIdle();

    protected:
        GIDeviceVk() = default;
        static void SetupDeviceQueues(
            VkPhysicalDevice physicalDevice,
            std::vector<VkDeviceQueueCreateInfo> queueCreateInfos,
            uint32_t* graphicsQueueFamily,
            uint32_t* computeQueueFamily,
            uint32_t* transferQueueFamily
        );

    private:
        SharedPtr<GIInstanceVk> mInstance;
        VkPhysicalDevice mPhysicalDeviceHandle = VK_NULL_HANDLE;
        VkDevice mLogicalDeviceHandle = VK_NULL_HANDLE;
        std::vector<const char*> mEnabledExtensionNames;

        SharedPtr<GICommandQueueVk> mGraphicsQueue;
        SharedPtr<GICommandQueueVk> mComputeQueue;
        SharedPtr<GICommandQueueVk> mTransferQueue;
        SharedPtr<GICommandQueueVk> mPresentQueue;
    }; 
}