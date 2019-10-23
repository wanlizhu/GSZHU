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

    extern std::pair<VkInstance, VkDebugUtilsMessengerEXT>
    CreateInstance(const STRLIST& extensions, 
                   const STRLIST& layers,
                   bool enableValidationLayers);
    
    extern VkPhysicalDeviceFeatures Unpack(const LIST<EDeviceFeature>& features);

    extern VkPhysicalDevice SelectPhysicalDevice(VkInstance instance,
                                                 VkPhysicalDeviceFeatures features);




    VulkanDevice::~VulkanDevice()
    {
        Destroy();
    }

    bool VulkanDevice::Initialize(const InitializeData& data)
    {
        return Initialize(VK_NULL_HANDLE, VK_NULL_HANDLE, data);
    }

    bool VulkanDevice::Initialize(VkInstance existedInstance,
                                  VkPhysicalDevice physicalDevice, 
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

            auto result = CreateInstance(instanceExtensions, instanceLayers, __enableValidationLayers);
            mVkInstance = result.first;
            mVkDebugUtilsMessengerHandle = result.second;
            if (mVkInstance == VK_NULL_HANDLE)
            {
                Destroy();
                return false;
            }
        }
        else
        {
            mVkInstance = existedInstance;
            mVkDebugUtilsMessengerHandle = VK_NULL_HANDLE;
        }

        // enumerate physical devices and select the best one
        // select if needed!!!
        if (physicalDevice == VK_NULL_HANDLE)
        {
            VkPhysicalDeviceFeatures features = Unpack(data.deviceFeatures);
            mVkPhysicalDevice = SelectPhysicalDevice(mVkInstance, features);
            if (mVkPhysicalDevice == VK_NULL_HANDLE)
            {
                Destroy();
                return false;
            }
        }
        else
        {
            mVkPhysicalDevice = physicalDevice;
        }

        // enumerate command queues and select the best ones

        // create logical device

        return true;
    }

    void VulkanDevice::Destroy()
    {

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