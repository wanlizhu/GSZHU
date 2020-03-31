#include "GIDeviceVk.h"
#include "GIDeviceQueueVk.h"
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
        , mPhysicalDevice(physicalDevice)
    {
        assert(instance->IsValid());
        assert(physicalDevice != VK_NULL_HANDLE);

        vkGetPhysicalDeviceProperties(mPhysicalDevice, &mPhysicalDeviceInfo.properties);
        vkGetPhysicalDeviceFeatures(mPhysicalDevice, &mPhysicalDeviceInfo.features);
        vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &mPhysicalDeviceInfo.memoryProperties);

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensionCount, nullptr);
        mPhysicalDeviceInfo.supportedExtensions.resize(extensionCount);
        vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensionCount, mPhysicalDeviceInfo.supportedExtensions.data());
        mOptionalExtensions.Initialize(mPhysicalDeviceInfo.supportedExtensions);

        uint32_t familyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &familyCount, nullptr);
        mPhysicalDeviceInfo.supportedQueueFamilies.resize(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &familyCount, mPhysicalDeviceInfo.supportedQueueFamilies.data());

        VK_CHECK(vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mLogicalDevice));
    }

    GIDeviceVk::~GIDeviceVk()
    {
        if (IsValid())
        {
            vkDestroyDevice(mLogicalDevice, nullptr);
            mLogicalDevice = VK_NULL_HANDLE;
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
        return mLogicalDevice;
    }

    bool GIDeviceVk::IsValid() const
    {
        return mLogicalDevice != VK_NULL_HANDLE;
    }

    void GIDeviceVk::SetupPresentQueue(VkSurfaceKHR surface)
    {
        const auto CheckSupportsPresent = [&](const SharedPtr<GIDeviceQueueVk>& queue) {
            if (!queue)
                return false;
            
            VkBool32 supportsPresent = VK_FALSE;
            uint32_t familyIndex = queue->GetFamilyIndex();
            VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice, familyIndex, surface, &supportsPresent));
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
        return mPhysicalDevice;
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
        return mLogicalDevice;
    }

    GIDeviceContextVk* GIDeviceVk::GetDC() const
    {
        return mCurrentDC.lock().get();
    }

    SharedPtr<GIDeviceQueueVk> GIDeviceVk::GetGraphicsQueue() const
    {
        return mGraphicsQueue;
    }

    SharedPtr<GIDeviceQueueVk> GIDeviceVk::GetPresentQueue() const
    {
        return mPresentQueue;
    }

    SharedPtr<GIDeviceQueueVk> GIDeviceVk::GetComputeQueue() const
    {
        return mComputeQueue;
    }

    SharedPtr<GIDeviceQueueVk> GIDeviceVk::GetTransferQueue() const
    {
        return mTransferQueue;
    }
    
    void GIDeviceVk::WaitIdle()
    {
        vkDeviceWaitIdle(mLogicalDevice);
    }

    GIDeviceBuilderVk::GIDeviceBuilderVk(SharedPtr<GIInstanceVk> instance)
        : mInstance(instance)
    {
        mCreateInfo = {};
        mCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.flags = 0;
    }

    GIDeviceBuilderVk& GIDeviceBuilderVk::SetPhysicalDevice(VkPhysicalDevice physicalDevice)
    {
        mPhysicalDevice = physicalDevice;
        assert(mPhysicalDevice != VK_NULL_HANDLE);
        vkGetPhysicalDeviceFeatures(mPhysicalDevice, &mSupportedFeatures);

        uint32_t extensionCount = 0;
        VK_CHECK(vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensionCount, nullptr));
        mSupportedExtensions.resize(extensionCount);
        VK_CHECK(vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensionCount, mSupportedExtensions.data()));

        uint32_t familyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &familyCount, nullptr);
        mSupportedQueueFamilies.resize(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &familyCount, mSupportedQueueFamilies.data());

        return *this;
    }

    GIDeviceBuilderVk& GIDeviceBuilderVk::EnableExtension(const char* name, bool compulsory)
    {
        if (std::find_if(
            mSupportedExtensions.begin(),
            mSupportedExtensions.end(),
            [&](const VkExtensionProperties& item) {
                return strcmp(item.extensionName, name) == 0;
            }) == mSupportedExtensions.end())
        {
            if (compulsory)
            {
                LOG_ERROR("Compulsory extension '%s' is not supported.\n", name);
            }
            else
            {
                LOG_WARNING("Optional extension '%s' is not supported.\n", name);
            }
        }
        else
        {
            mEnabledExtensions.push_back(name);
        }

        return *this;
    }

    GIDeviceBuilderVk& GIDeviceBuilderVk::EnableExtensions(const std::vector<const char*>& names)
    {
        for (const auto& name : names)
        {
            EnableExtension(name, true);
        }
        return *this;
    }

    GIDeviceBuilderVk& GIDeviceBuilderVk::EnableFeatures(const VkPhysicalDeviceFeatures& features)
    {
        size_t count = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);
        VkBool32* src = (VkBool32*)&mSupportedFeatures;
        VkBool32* dst = (VkBool32*)&features;

        for (size_t i = 0; i < count; i++)
        {
            if (*(dst + i) == VK_TRUE && *(src + i) == VK_FALSE)
            {
                return *this;
            }
        }

        mEnabledFeatures = features;
        mCreateInfo.pEnabledFeatures = &mEnabledFeatures;

        return *this;
    }

    GIDeviceBuilderVk& GIDeviceBuilderVk::CreateGraphicsQueue()
    {
        assert(mSupportedQueueFamilies.size() > 0);
        static float queuePriorities[1] = { 0.f };

        for (uint32_t i = 0; i < mSupportedQueueFamilies.size(); i++)
        {
            if ((mSupportedQueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
            {
                if (!FindFromQueueCreateInfos(i))
                {
                    VkDeviceQueueCreateInfo queueCreateInfo = {};
                    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    queueCreateInfo.queueFamilyIndex = i;
                    queueCreateInfo.queueCount = 1;
                    queueCreateInfo.pQueuePriorities = queuePriorities;
                    mQueueCreateInfos.emplace_back(queueCreateInfo);
                }
            }
        }

        return *this;
    }

    GIDeviceBuilderVk& GIDeviceBuilderVk::CreateComputeQueue()
    {
        assert(mSupportedQueueFamilies.size() > 0);
        static float queuePriorities[1] = { 0.f };

        for (uint32_t i = 0; i < mSupportedQueueFamilies.size(); i++)
        {
            if ((mSupportedQueueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT))
            {
                if (!FindFromQueueCreateInfos(i))
                {
                    VkDeviceQueueCreateInfo queueCreateInfo = {};
                    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    queueCreateInfo.queueFamilyIndex = i;
                    queueCreateInfo.queueCount = 1;
                    queueCreateInfo.pQueuePriorities = queuePriorities;
                    mQueueCreateInfos.emplace_back(queueCreateInfo);
                }
            }
        }

        return *this;
    }

    GIDeviceBuilderVk& GIDeviceBuilderVk::CreateTransferQueue()
    {
        assert(mSupportedQueueFamilies.size() > 0);
        static float queuePriorities[1] = { 0.f };

        for (uint32_t i = 0; i < mSupportedQueueFamilies.size(); i++)
        {
            if ((mSupportedQueueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT))
            {
                if (!FindFromQueueCreateInfos(i))
                {
                    VkDeviceQueueCreateInfo queueCreateInfo = {};
                    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    queueCreateInfo.queueFamilyIndex = i;
                    queueCreateInfo.queueCount = 1;
                    queueCreateInfo.pQueuePriorities = queuePriorities;
                    mQueueCreateInfos.emplace_back(queueCreateInfo);
                }
            }
        }

        return *this;
    }

    bool GIDeviceBuilderVk::FindFromQueueCreateInfos(uint32_t familyIndex)
    {
        return std::find_if(
            mQueueCreateInfos.begin(),
            mQueueCreateInfos.end(),
            [&](const VkDeviceQueueCreateInfo& info) {
                return info.queueFamilyIndex == familyIndex;
            }) != mQueueCreateInfos.end();
    }

    SharedPtr<GIDeviceVk> GIDeviceBuilderVk::Build()
    {
        mCreateInfo.pEnabledFeatures = &mEnabledFeatures;
        mCreateInfo.enabledExtensionCount = (uint32_t)mEnabledExtensions.size();
        mCreateInfo.ppEnabledExtensionNames = mEnabledExtensions.data();
        if (mInstance->IsValidationLayerEnabled())
        {
            mCreateInfo.enabledLayerCount = (uint32_t)mInstance->mEnabledLayers.size();
            mCreateInfo.ppEnabledLayerNames = mInstance->mEnabledLayers.data();
        }
        mCreateInfo.queueCreateInfoCount = (uint32_t)mQueueCreateInfos.size();
        mCreateInfo.pQueueCreateInfos = mQueueCreateInfos.data();

        auto device = SharedPtr<GIDeviceVk>(new GIDeviceVk(
            mInstance,
            mPhysicalDevice,
            mCreateInfo));
        assert(device->IsValid());

        for (uint32_t i = 0; i < mCreateInfo.queueCreateInfoCount; i++)
        {
            const auto& queueFamilies = device->mPhysicalDeviceInfo.supportedQueueFamilies;
            uint32_t familyIndex = mCreateInfo.pQueueCreateInfos[i].queueFamilyIndex;
            VkQueue queue = VK_NULL_HANDLE;
            vkGetDeviceQueue(device->mLogicalDevice, familyIndex, 0, &queue);

            if (VK_QUEUE_GRAPHICS_BIT & queueFamilies[familyIndex].queueFlags)
            {
                device->mGraphicsQueue.reset(new GIDeviceQueueVk(device, queue, familyIndex));
                assert(device->mGraphicsQueue->IsValid());
            }

            if (VK_QUEUE_COMPUTE_BIT & queueFamilies[familyIndex].queueFlags)
            {
                device->mComputeQueue.reset(new GIDeviceQueueVk(device, queue, familyIndex));
                assert(device->mComputeQueue->IsValid());
            }

            if (VK_QUEUE_TRANSFER_BIT & queueFamilies[familyIndex].queueFlags)
            {
                device->mTransferQueue.reset(new GIDeviceQueueVk(device, queue, familyIndex));
                assert(device->mTransferQueue->IsValid());
            }
        }

        return device;
    }
}