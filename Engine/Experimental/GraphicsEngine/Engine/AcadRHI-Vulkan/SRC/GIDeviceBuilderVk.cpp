#include "GIDeviceBuilderVk.h"
#include "GIDeviceVk.h"
#include "GICommandQueueVk.h"

namespace AutoCAD::Graphics::Engine
{
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
        mPhysicalDeviceHandle = physicalDevice;
        assert(mPhysicalDeviceHandle != VK_NULL_HANDLE);
        vkGetPhysicalDeviceFeatures(mPhysicalDeviceHandle, &mSupportedFeatures);

        uint32_t extensionCount = 0;
        VK_CHECK(vkEnumerateDeviceExtensionProperties(mPhysicalDeviceHandle, nullptr, &extensionCount, nullptr));
        mSupportedExtensions.resize(extensionCount);
        VK_CHECK(vkEnumerateDeviceExtensionProperties(mPhysicalDeviceHandle, nullptr, &extensionCount, mSupportedExtensions.data()));

        uint32_t familyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDeviceHandle, &familyCount, nullptr);
        mSupportedQueueFamilies.resize(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDeviceHandle, &familyCount, mSupportedQueueFamilies.data());

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
            mPhysicalDeviceHandle,
            mCreateInfo));
        assert(device->IsValid());

        for (uint32_t i = 0; i < mCreateInfo.queueCreateInfoCount; i++)
        {
            const auto& queueFamilies = device->mPhysicalDeviceInfo.supportedQueueFamilies;
            uint32_t familyIndex = mCreateInfo.pQueueCreateInfos[i].queueFamilyIndex;
            VkQueue queue = VK_NULL_HANDLE;
            vkGetDeviceQueue(device->mLogicalDeviceHandle, familyIndex, 0, &queue);

            if (VK_QUEUE_GRAPHICS_BIT & queueFamilies[familyIndex].queueFlags)
            {
                device->mGraphicsQueue.reset(new GICommandQueueVk(device, queue, familyIndex));
                assert(device->mGraphicsQueue->IsValid());
            }

            if (VK_QUEUE_COMPUTE_BIT & queueFamilies[familyIndex].queueFlags)
            {
                device->mComputeQueue.reset(new GICommandQueueVk(device, queue, familyIndex));
                assert(device->mComputeQueue->IsValid());
            }

            if (VK_QUEUE_TRANSFER_BIT & queueFamilies[familyIndex].queueFlags)
            {
                device->mTransferQueue.reset(new GICommandQueueVk(device, queue, familyIndex));
                assert(device->mTransferQueue->IsValid());
            }
        }

        return device;
    }
}