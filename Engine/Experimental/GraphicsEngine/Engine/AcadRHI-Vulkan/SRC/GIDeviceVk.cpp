#include "GIDeviceVk.h"
#include "GICommandQueueVk.h"
#include <assert.h>
#include <algorithm>

namespace AutoCAD::Graphics::Engine
{
    GIDeviceVk::GIDeviceVk(
        SharedPtr<GIInstanceVk> instance,
        VkPhysicalDevice physicalDevice,
        VkDeviceCreateInfo& createInfo
    )
        : mInstance(instance)
        , mPhysicalDeviceHandle(physicalDevice)
    {
        assert(instance->IsValid());
        assert(physicalDevice != VK_NULL_HANDLE);

        vkGetPhysicalDeviceProperties(mPhysicalDeviceHandle, &mPhysicalDeviceInfo.properties);
        vkGetPhysicalDeviceFeatures(mPhysicalDeviceHandle, &mPhysicalDeviceInfo.features);
        vkGetPhysicalDeviceMemoryProperties(mPhysicalDeviceHandle, &mPhysicalDeviceInfo.memoryProperties);

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(mPhysicalDeviceHandle, nullptr, &extensionCount, nullptr);
        mPhysicalDeviceInfo.supportedExtensions.resize(extensionCount);
        vkEnumerateDeviceExtensionProperties(mPhysicalDeviceHandle, nullptr, &extensionCount, mPhysicalDeviceInfo.supportedExtensions.data());
        mOptionalExtensions.Initialize(mPhysicalDeviceInfo.supportedExtensions);

        uint32_t familyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDeviceHandle, &familyCount, nullptr);
        mPhysicalDeviceInfo.supportedQueueFamilies.resize(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDeviceHandle, &familyCount, mPhysicalDeviceInfo.supportedQueueFamilies.data());

        VK_CHECK(vkCreateDevice(mPhysicalDeviceHandle, &createInfo, nullptr, &mLogicalDeviceHandle));
    }

    GIDeviceVk::~GIDeviceVk()
    {
        if (IsValid())
        {
            vkDestroyDevice(mLogicalDeviceHandle, nullptr);
            mLogicalDeviceHandle = VK_NULL_HANDLE;
            mInstance.reset();
        }
    }

    OptionalDeviceExtensions::OptionalDeviceExtensions()
        : packed(0)
    {
        static_assert(sizeof(packed) == sizeof(OptionalDeviceExtensions));
    }

    void OptionalDeviceExtensions::Initialize(const std::vector<VkExtensionProperties> extensions)
    {
        auto HasExtension = [&](const char* name) {
            return std::find_if(
                extensions.begin(),
                extensions.end(),
                [&](const VkExtensionProperties& prop) {
                    return strcmp(name, prop.extensionName) == 0;
                }) != extensions.end();
        };

        KHRMaintenance1 = HasExtension(VK_KHR_MAINTENANCE1_EXTENSION_NAME);
        KHRMaintenance2 = HasExtension(VK_KHR_MAINTENANCE2_EXTENSION_NAME);
        KHRDedicatedAllocation = HasExtension(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
        EXTValidationCache = HasExtension(VK_EXT_VALIDATION_CACHE_EXTENSION_NAME);
        AMDBufferMarker = HasExtension(VK_AMD_BUFFER_MARKER_EXTENSION_NAME);
        NVDiagnosticCheckpoints = HasExtension(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
        GoogleDisplayTiming = HasExtension(VK_GOOGLE_DISPLAY_TIMING_EXTENSION_NAME);
        YcbcrSampler = HasExtension(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME) &&
            HasExtension(VK_KHR_BIND_MEMORY_2_EXTENSION_NAME) &&
            HasExtension(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
        MemoryPriority = HasExtension(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME);
        DriverProperties = HasExtension(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME);
        EXTFragmentDensityMap = HasExtension(VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME);
    }

    GIDeviceVk::operator const VkDevice& () const
    {
        return mLogicalDeviceHandle;
    }

    bool GIDeviceVk::IsValid() const
    {
        return mLogicalDeviceHandle != VK_NULL_HANDLE;
    }

    void GIDeviceVk::SetupPresentQueue(VkSurfaceKHR surface)
    {
        const auto CheckSupportsPresent = [&](const SharedPtr<GICommandQueueVk>& queue) {
            if (!queue)
                return false;
            
            VkBool32 supportsPresent = VK_FALSE;
            uint32_t familyIndex = queue->GetFamilyIndex();
            VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDeviceHandle, familyIndex, surface, &supportsPresent));
            return supportsPresent == VK_TRUE;
        };

        if (CheckSupportsPresent(mGraphicsQueue))
        {
            mPresentQueue = mGraphicsQueue;
            return;
        }
        LOG_WARNING("Graphics queue does not support present!\n");

        if (CheckSupportsPresent(mComputeQueue))
        {
            mPresentQueue = mComputeQueue;
            LOG_WARNING("Take compute queue as present queue\n");
            return;
        }

        if (CheckSupportsPresent(mTransferQueue))
        {
            mPresentQueue = mTransferQueue;
            LOG_WARNING("Take transfer queue as present queue\n");
            return;
        }

        LOG_ERROR("No device queue supports present to surface!\n");
    }

    void GIDeviceVk::SetCurrentDC(WeakPtr<GIDeviceContextVk> dc)
    {
        mCurrentDC = dc;
    }

    SharedPtr<GIInstanceVk> const& GIDeviceVk::GetInstance() const
    {
        return mInstance;
    }

    uint32_t GIDeviceVk::GetMemoryTypeIndex(uint32_t typeFilters, VkMemoryPropertyFlags properties) const
    {
        return 0; // TODO
    }

    VkPhysicalDevice GIDeviceVk::GetPhysicalDevice() const
    {
        return mPhysicalDeviceHandle;
    }

    const GIDeviceVk::PhysicalDeviceInfo& GIDeviceVk::GetPhysicalDeviceInfo() const
    {
        return mPhysicalDeviceInfo;
    }

    OptionalDeviceExtensions const& GIDeviceVk::GetOptionalExtensions() const
    {
        return mOptionalExtensions;
    }

    VkDevice GIDeviceVk::GetLogicalDevice() const
    {
        return mLogicalDeviceHandle;
    }

    GIDeviceContextVk* GIDeviceVk::GetDC() const
    {
        return mCurrentDC.lock().get();
    }

    SharedPtr<GICommandQueueVk> GIDeviceVk::GetGraphicsQueue() const
    {
        return mGraphicsQueue;
    }

    SharedPtr<GICommandQueueVk> GIDeviceVk::GetPresentQueue() const
    {
        return mPresentQueue;
    }

    SharedPtr<GICommandQueueVk> GIDeviceVk::GetComputeQueue() const
    {
        return mComputeQueue;
    }

    SharedPtr<GICommandQueueVk> GIDeviceVk::GetTransferQueue() const
    {
        return mTransferQueue;
    }
    
    void GIDeviceVk::WaitIdle()
    {
        vkDeviceWaitIdle(mLogicalDeviceHandle);
    }
}