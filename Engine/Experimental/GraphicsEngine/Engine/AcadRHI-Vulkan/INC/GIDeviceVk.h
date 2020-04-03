#pragma once

#include "GIPlatformVk.h"
#include "GICommonVk.h"
#include "GIInstanceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GICommandQueueVk;
    class GIDeviceContextVk;

    struct OptionalDeviceExtensions
    {
        union
        {
            struct
            {
                uint32_t KHRMaintenance1 : 1;
                uint32_t KHRMaintenance2 : 1;
                uint32_t KHRDedicatedAllocation : 1;
                uint32_t EXTValidationCache : 1;
                uint32_t AMDBufferMarker : 1;
                uint32_t NVDiagnosticCheckpoints : 1;
                uint32_t GoogleDisplayTiming : 1;
                uint32_t YcbcrSampler : 1;
                uint32_t MemoryPriority : 1;
                uint32_t DriverProperties : 1;
                uint32_t EXTFragmentDensityMap : 1;
            };
            uint32_t packed = 0;
        };

    private:
        friend class GIDeviceVk;

        OptionalDeviceExtensions();
        void Initialize(const std::vector<VkExtensionProperties> extensions);
    };

    class GIDeviceVk : public std::enable_shared_from_this<GIDeviceVk>
    {
        friend class GIDeviceBuilderVk;
    public:
        struct PhysicalDeviceInfo
        {
            VkPhysicalDeviceProperties properties;
            VkPhysicalDeviceFeatures features;
            VkPhysicalDeviceMemoryProperties memoryProperties;
            std::vector<VkQueueFamilyProperties> supportedQueueFamilies;
            std::vector<VkExtensionProperties> supportedExtensions;
        };

        virtual ~GIDeviceVk();
        operator const VkDevice& () const;
        bool IsValid() const;
        void SetupPresentQueue(VkSurfaceKHR surface);
        void SetCurrentDC(WeakPtr<GIDeviceContextVk> dc);
        
        SharedPtr<GIInstanceVk> const& GetInstance() const;
        uint32_t GetMemoryTypeIndex(uint32_t typeFilters, VkMemoryPropertyFlags properties) const;
        VkPhysicalDevice GetPhysicalDevice() const;
        PhysicalDeviceInfo const& GetPhysicalDeviceInfo() const;
        OptionalDeviceExtensions const& GetOptionalExtensions() const;

        VkDevice GetLogicalDevice() const;
        GIDeviceContextVk* GetDC() const;
        SharedPtr<GICommandQueueVk> GetGraphicsQueue() const;
        SharedPtr<GICommandQueueVk> GetComputeQueue() const;
        SharedPtr<GICommandQueueVk> GetTransferQueue() const;
        SharedPtr<GICommandQueueVk> GetPresentQueue() const;

        void WaitIdle();

    protected:
        GIDeviceVk(
            SharedPtr<GIInstanceVk> instance,
            VkPhysicalDevice physicalDevice,
            VkDeviceCreateInfo& createInfo
        );

    private:
        SharedPtr<GIInstanceVk> mInstance;
        VkPhysicalDevice mPhysicalDeviceHandle = VK_NULL_HANDLE;
        PhysicalDeviceInfo mPhysicalDeviceInfo;
        OptionalDeviceExtensions mOptionalExtensions;

        VkDevice mLogicalDeviceHandle = VK_NULL_HANDLE;
        WeakPtr<GIDeviceContextVk> mCurrentDC;
        SharedPtr<GICommandQueueVk> mGraphicsQueue;
        SharedPtr<GICommandQueueVk> mComputeQueue;
        SharedPtr<GICommandQueueVk> mTransferQueue;
        SharedPtr<GICommandQueueVk> mPresentQueue;
    }; 
}