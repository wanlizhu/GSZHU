#pragma once

#include "VulkanDevice.h"

namespace GE2::RHI
{
    static const STRLIST __validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

#ifdef _DEBUG
    bool __enableValidationLayers = true;
#else
    bool __enableValidationLayers = false;
#endif

    extern VkInstance CreateInstance(const STRLIST& extensions,
                                     const STRLIST& layers,
                                     bool enableValidationLayers,
                                     VkDebugUtilsMessengerEXT* messenger);
    
    extern VkPhysicalDeviceFeatures Unpack(const LIST<EDeviceFeature>& features);

    extern VkPhysicalDevice FindPhysicalDevice(VkInstance instance,
                                               VkPhysicalDeviceFeatures features,
                                               QueueFamilies* queueFamilies);

    extern void FindQueueFamilies(VkPhysicalDevice physicalDevice,
                                  QueueFamilies* queueFamilies);

    extern VkDevice CreateLogicalDevice(VkPhysicalDevice physicalDevice,
                                        const QueueFamilies& queueFamilies,
                                        LIST<VkQueue>* graphicsQueues,
                                        LIST<VkQueue>* computeQueues,
                                        LIST<VkQueue>* transferQueues);








    VulkanDevice::~VulkanDevice()
    {
        Destroy(); 
    }

    bool VulkanDevice::Initialize(const InitializeData& data)
    {
        return Initialize(VK_NULL_HANDLE,
                          VK_NULL_HANDLE,
                          QueueFamilies(),
                          data);
    }

    bool VulkanDevice::Initialize(VkInstance existedInstance,
                                  VkPhysicalDevice physicalDevice, 
                                  QueueFamilies queueFamilies,
                                  const InitializeData& data)
    {
        // create instance, load extensions and setup validation layers
        // create if needed!!!
        if (existedInstance == VK_NULL_HANDLE)
        {
            STRLIST instanceExtensions = data.instanceExtensions;
            STRLIST instanceLayers = data.instanceLayers;
            if (__enableValidationLayers)
                instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

            mVkInstance = CreateInstance(instanceExtensions,
                                         instanceLayers,
                                         __enableValidationLayers,
                                         &mVkDebugUtilsMessengerHandle);
            if (mVkInstance == VK_NULL_HANDLE)
            {
                Destroy();
                return false;
            }
        }
        else
        {
            mIsExternalInstance = true;
            mVkInstance = existedInstance;
            mVkDebugUtilsMessengerHandle = VK_NULL_HANDLE;
        }

        // enumerate physical devices and select the best one
        // select if needed!!!
        if (physicalDevice == VK_NULL_HANDLE)
        {
            VkPhysicalDeviceFeatures features = RHI::Unpack(data.deviceFeatures);
            mVkPhysicalDevice = FindPhysicalDevice(mVkInstance,
                                                   features,
                                                   &queueFamilies);
            if (mVkPhysicalDevice == VK_NULL_HANDLE ||
                !queueFamilies.isCapable())
            {
                Destroy();
                return false;
            }
        }
        else
        {
            mVkPhysicalDevice = physicalDevice;
            if (queueFamilies.empty())
                FindQueueFamilies(mVkPhysicalDevice, &queueFamilies);
            if (!queueFamilies.isCapable())
            {
                Destroy();
                return false;
            }
        }

        // create logical device
        mVkDevice = CreateLogicalDevice(mVkPhysicalDevice,
                                        queueFamilies,
                                        &mVkGraphicsQueues,
                                        &mVkComputeQueues,
                                        &mVkTransferQueues);
        if (mVkDevice == VK_NULL_HANDLE ||
            (mVkGraphicsQueues.empty() &&
             mVkComputeQueues.empty()  &&
             mVkTransferQueues.empty()))
        {
            Destroy();
            return false;
        }

        return true;
    }

    void VulkanDevice::Destroy()
    {
        // destroy logic device
        if (mVkDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(mVkDevice, nullptr);
            mVkDevice = VK_NULL_HANDLE;
        }

        // destroy VkDebugUtilsMessengerEXT
        if (__enableValidationLayers && 
            mVkInstance != VK_NULL_HANDLE &&
            mVkDebugUtilsMessengerHandle != VK_NULL_HANDLE)
        {
            INSTANCE_PROC(mVkInstance, vkDestroyDebugUtilsMessengerEXT)
                .call<void>(mVkInstance, mVkDebugUtilsMessengerHandle, nullptr);
            mVkDebugUtilsMessengerHandle = VK_NULL_HANDLE;
        }

        // destroy VkInstance
        // VkPhysicalDevice will be implicitly destroyed when the VkInstance is destroyed
        if (mVkInstance && !mIsExternalInstance)
        {
            vkDestroyInstance(mVkInstance, nullptr);
            mVkInstance = VK_NULL_HANDLE;
        }

        mVkGraphicsQueues.clear();
        mVkComputeQueues.clear();
        mVkTransferQueues.clear();
    }

    CSTR VulkanDevice::GetName() const
    {
        return mName.c_str();
    }

    HANDLE VulkanDevice::GetHandle() const
    {
        return static_cast<HANDLE>(mVkDevice);
    }
}