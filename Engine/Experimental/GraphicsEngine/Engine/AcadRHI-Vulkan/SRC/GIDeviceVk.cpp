#include "GIDeviceVk.h"
#include "GICommandQueueVk.h"
#include <assert.h>
#include <algorithm>

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIDeviceVk> GIDeviceVk::Create(
        SharedPtr<GIInstanceVk> instance,
        VkPhysicalDevice physicalDevice
    )
    {
        assert(instance->IsValid());
        assert(physicalDevice != VK_NULL_HANDLE);

        VkDevice logicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties properties = {};
        VkPhysicalDeviceFeatures features = {};
        VkPhysicalDeviceMemoryProperties memoryProps = {};
        uint32_t graphicsQueueFamily = 0;
        uint32_t computeQueueFamily = 0;
        uint32_t transferQueueFamily = 0;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::vector<VkExtensionProperties> supportedExtensions;
        std::vector<VkQueueFamilyProperties> supportedQueueFamilies;
        std::vector<const char*> supportedExtensionNames;

        SetupDeviceQueues(
            physicalDevice,
            queueCreateInfos,
            &graphicsQueueFamily,
            &computeQueueFamily,
            &transferQueueFamily
        );

        vkGetPhysicalDeviceProperties(physicalDevice, &properties);
        vkGetPhysicalDeviceFeatures(physicalDevice, &features);
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProps);

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
        supportedExtensions.resize(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, supportedExtensions.data());
        std::for_each(supportedExtensions.begin(), 
                      supportedExtensions.end(), 
                      [&](const VkExtensionProperties& prop) {
                          supportedExtensionNames.push_back(prop.extensionName);
                      });

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.enabledLayerCount = (uint32_t)instance->GetEnabledLayers().size();
        createInfo.ppEnabledLayerNames = instance->GetEnabledLayers().data();
        createInfo.enabledExtensionCount = (uint32_t)supportedExtensionNames.size();
        createInfo.ppEnabledExtensionNames = supportedExtensionNames.data();
        createInfo.pEnabledFeatures = &features;
        VK_CHECK(vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice));

        SharedPtr<GIDeviceVk> result(new GIDeviceVk());
        result->mInstance = instance;
        result->mPhysicalDeviceHandle = physicalDevice;
        result->mLogicalDeviceHandle = logicalDevice;
        result->mEnabledExtensionNames = supportedExtensionNames;

        VkQueue graphicsQueueHandle = VK_NULL_HANDLE;
        vkGetDeviceQueue(logicalDevice, graphicsQueueFamily, 0, &graphicsQueueHandle);
        result->mGraphicsQueue = GICommandQueueVk::Create(result, graphicsQueueHandle, graphicsQueueFamily);

        VkQueue computeQueueHandle = VK_NULL_HANDLE;
        vkGetDeviceQueue(logicalDevice, computeQueueFamily, 0, &computeQueueHandle);
        result->mGraphicsQueue = GICommandQueueVk::Create(result, computeQueueHandle, computeQueueFamily);

        VkQueue transferQueueHandle = VK_NULL_HANDLE;
        vkGetDeviceQueue(logicalDevice, transferQueueFamily, 0, &transferQueueHandle);
        result->mGraphicsQueue = GICommandQueueVk::Create(result, transferQueueHandle, transferQueueFamily);

        return result;
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

    GIDeviceVk::operator const VkDevice& () const
    {
        return mLogicalDeviceHandle;
    }

    bool GIDeviceVk::IsValid() const
    {
        return mLogicalDeviceHandle != VK_NULL_HANDLE;
    }

    bool GIDeviceVk::IsExtensionEnabled(const char* name) const
    {
        return std::find_if(
            mEnabledExtensionNames.begin(),
            mEnabledExtensionNames.end(),
            [&](const char* extension) {
                return std::strcmp(extension, name) == 0;
            }) != mEnabledExtensionNames.end();
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


    VkDevice GIDeviceVk::GetLogicalDevice() const
    {
        return mLogicalDeviceHandle;
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

    void GIDeviceVk::SetupDeviceQueues(
        VkPhysicalDevice physicalDevice,
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos,
        uint32_t* graphicsQueueFamily,
        uint32_t* computeQueueFamily,
        uint32_t* transferQueueFamily
    )
    {
        uint32_t dummy[3];
        if (graphicsQueueFamily == nullptr)
            graphicsQueueFamily = &dummy[0];
        if (computeQueueFamily == nullptr)
            computeQueueFamily = &dummy[1];
        if (transferQueueFamily == nullptr)
            transferQueueFamily = &dummy[2];

        *graphicsQueueFamily = UINT_MAX;
        *computeQueueFamily = UINT_MAX;
        *transferQueueFamily = UINT_MAX;

        uint32_t familyCount = 0;
        std::vector<VkQueueFamilyProperties> queueFamilyProps;
        static float queuePriorities[1] = { 0.f };
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, nullptr);
        queueFamilyProps.resize(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, queueFamilyProps.data());

        for (uint32_t i = 0; i < queueFamilyProps.size(); i++)
        {
            if (*graphicsQueueFamily != UINT_MAX &&
                *computeQueueFamily != UINT_MAX &&
                *transferQueueFamily != UINT_MAX)
            {
                break;
            }

            if (*graphicsQueueFamily == UINT_MAX 
                && (queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
            {
                VkDeviceQueueCreateInfo queueCreateInfo = {};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = i;
                queueCreateInfo.queueCount = 1;
                queueCreateInfo.pQueuePriorities = queuePriorities;
                queueCreateInfos.emplace_back(queueCreateInfo);
                *graphicsQueueFamily = i;
            }

            if (*computeQueueFamily == UINT_MAX
                && (queueFamilyProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT))
            {
                VkDeviceQueueCreateInfo queueCreateInfo = {};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = i;
                queueCreateInfo.queueCount = 1;
                queueCreateInfo.pQueuePriorities = queuePriorities;
                queueCreateInfos.emplace_back(queueCreateInfo);
                *computeQueueFamily = i;
            }

            if (*transferQueueFamily == UINT_MAX
                && (queueFamilyProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT))
            {
                VkDeviceQueueCreateInfo queueCreateInfo = {};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = i;
                queueCreateInfo.queueCount = 1;
                queueCreateInfo.pQueuePriorities = queuePriorities;
                queueCreateInfos.emplace_back(queueCreateInfo);
                *transferQueueFamily = i;
            }
        }

        std::sort(
            queueCreateInfos.begin(),
            queueCreateInfos.end(), 
            [](const VkDeviceQueueCreateInfo& info1, const VkDeviceQueueCreateInfo& info2) {
                return info1.queueFamilyIndex < info2.queueFamilyIndex;
            });
        auto uniqueEnd = std::unique(queueCreateInfos.begin(),
                                     queueCreateInfos.end(),
                                     [](const VkDeviceQueueCreateInfo& info1, const VkDeviceQueueCreateInfo& info2) {
                                         return info1.queueFamilyIndex == info2.queueFamilyIndex;
                                     });
        if (uniqueEnd != queueCreateInfos.end())
        {
            queueCreateInfos.erase(uniqueEnd);
        }
    }
}