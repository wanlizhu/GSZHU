#pragma once

#include "GIPlatformVk.h"
#include "GICommonVk.h"
#include "VKInstance.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceQueueVk;

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
        
        SharedPtr<VKInstance> const& GetInstance() const;
        VkPhysicalDevice GetPhysicalDevice() const;
        PhysicalDeviceInfo const& GetPhysicalDeviceInfo() const;
        OptionalDeviceExtensions const& GetOptionalExtensions() const;

        VkDevice GetLogicalDevice() const;
        SharedPtr<GIDeviceQueueVk> GetGraphicsQueue() const;
        SharedPtr<GIDeviceQueueVk> GetComputeQueue() const;
        SharedPtr<GIDeviceQueueVk> GetTransferQueue() const;
        SharedPtr<GIDeviceQueueVk> GetPresentQueue() const;

        void WaitIdle();

    protected:
        GIDeviceVk(
            SharedPtr<VKInstance> instance,
            VkPhysicalDevice physicalDevice,
            VkDeviceCreateInfo& createInfo);

        GIDeviceVk(const GIDeviceVk&) = delete;
        GIDeviceVk(GIDeviceVk&&) = default;
        GIDeviceVk& operator=(const GIDeviceVk&) = delete;
        GIDeviceVk& operator=(GIDeviceVk&&) = default;

    private:
        SharedPtr<VKInstance> mInstance;
        VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
        PhysicalDeviceInfo mPhysicalDeviceInfo;
        OptionalDeviceExtensions mOptionalExtensions;

        VkDevice mLogicalDevice = VK_NULL_HANDLE;
        SharedPtr<GIDeviceQueueVk> mGraphicsQueue;
        SharedPtr<GIDeviceQueueVk> mComputeQueue;
        SharedPtr<GIDeviceQueueVk> mTransferQueue;
        SharedPtr<GIDeviceQueueVk> mPresentQueue;
    }; 

    class GIDeviceBuilderVk
    {
    public:
        GIDeviceBuilderVk(SharedPtr<VKInstance> instance);
        
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
        SharedPtr<VKInstance> mInstance;
        VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
        VkDeviceCreateInfo mCreateInfo = {};

        VkPhysicalDeviceFeatures mSupportedFeatures = {};
        std::vector<VkExtensionProperties> mSupportedExtensions;
        std::vector<VkQueueFamilyProperties> mSupportedQueueFamilies;

        VkPhysicalDeviceFeatures mEnabledFeatures = {};
        std::vector<const char*> mEnabledExtensions;
        std::vector<VkDeviceQueueCreateInfo> mQueueCreateInfos;
    };
}