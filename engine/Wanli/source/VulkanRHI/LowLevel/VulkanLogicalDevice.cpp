#include "VulkanRHI/LowLevel/VulkanLogicalDevice.h"
#include "VulkanRHI/LowLevel/VulkanInstance.h"
#include "VulkanRHI/LowLevel/VulkanPhysicalDevice.h"
#include "VulkanRHI/LowLevel/VulkanSurface.h"
#include "Utilities/Log.h"

namespace Wanli
{
    const PCCHArray VulkanLogicalDevice::kDeviceValidationLayers =
    {
    #if PLATFORM_WINDOWS
        "VK_LAYER_LUNARG_standard_validation",
    #elif PLATFORM_IOS
        "MoltenVK",
    #elif PLATFORM_MAC
        "VK_LAYER_LUNARG_standard_validation",
        "VK_LAYER_GOOGLE_unique_objects",
        "VK_LAYER_GOOGLE_threading",
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_object_tracker",
    #elif PLATFORM_ANDROID
        "VK_LAYER_GOOGLE_threading",
        "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_object_tracker",
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_GOOGLE_unique_objects",
    #elif PLATFORM_LINUX
        "VK_LAYER_GOOGLE_threading",
        "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_object_tracker",
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_GOOGLE_unique_objects",
        "VK_LAYER_LUNARG_core_validation",
    #else
    #endif
        nullptr
    };

    const PCCHArray VulkanLogicalDevice::kDeviceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME,
        "VK_KHR_maintenance1",
    };
    
    extern void UniquePCCHElements(PCCHArray& array);

    VulkanLogicalDevice::VulkanLogicalDevice(
        VulkanInstance* instance,
        VulkanPhysicalDevice* physicalDevice,
        VulkanSurface* surface)
        : mVkInstance(instance->shared_from_this())
        , mPhysicalDevice(physicalDevice->shared_from_this())
        , mSurface(surface->shared_from_this())
    {}

    VulkanLogicalDevice::~VulkanLogicalDevice()
    {
        Destroy();
    }

    bool VulkanLogicalDevice::Initialize()
    {
        try
        {
            CreateQueueIndices();
            AddEnabledLayersAndExtensions();
            AddEnabledFeatures();
            CreateLogicalDevice();
        }
        catch (VulkanError & e)
        {
            LOG_ERROR("!!!Failed to initialize VulkanLogicalDevice!!!:\n");
            LOG_ERROR("%s\n", e.what());
            return false;
        }
        return true;
    }

    void VulkanLogicalDevice::Destroy()
    {
        if (mLogicalDevice != VK_NULL_HANDLE)
        {
            vkDeviceWaitIdle(mLogicalDevice);
            vkDestroyDevice(mLogicalDevice, nullptr);
            mLogicalDevice = VK_NULL_HANDLE;
        }
    }

    void VulkanLogicalDevice::EnableExtension(const char* extension)
    {
        mEnabledExtensions.push_back(extension);
    }

    void VulkanLogicalDevice::EnableLayer(const char* layerName)
    {
        VulkanDeviceLayer layer(layerName, *mPhysicalDevice);
        mEnabledLayers.push_back(layerName);
        
        for (const auto& extension : layer.GetRequiredExtensions())
        {
            mEnabledExtensions.push_back(extension.extensionName);
        }
    }

    void VulkanLogicalDevice::CreateQueueIndices()
    {
        Uint queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(*mPhysicalDevice, &queueFamilyCount, nullptr);
        Array<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(*mPhysicalDevice, &queueFamilyCount, queueFamilies.data());

        Optional<Uint> graphicsFamily;
        Optional<Uint> presentFamily;
        Optional<Uint> computeFamily;
        Optional<Uint> transferFamily;

        for (Uint i = 0; i < queueFamilyCount; i++)
        {
            // Check for graphics support
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                graphicsFamily = i;
                mGraphicsFamily = i;
                mSupportedQueues |= VK_QUEUE_GRAPHICS_BIT;
            }

            // Check for presentation support
            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(*mPhysicalDevice, i, *mSurface, &presentSupport);
            if (queueFamilies[i].queueCount > 0 && presentSupport)
            {
                presentFamily = i;
                mPresentFamily = i;
            }

            // Check for compute support
            if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                computeFamily = i;
                mComputeFamily = i;
                mSupportedQueues |= VK_QUEUE_COMPUTE_BIT;
            }

            // Check for transfer support
            if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                transferFamily = i;
                mTransferFamily = i;
                mSupportedQueues |= VK_QUEUE_TRANSFER_BIT;
            }

            if (graphicsFamily &&
                presentFamily &&
                computeFamily &&
                transferFamily)
            {
                break;
            }
        }

        if (!graphicsFamily)
        {
            throw VulkanError("Failed to find queue family supporting VK_QUEUE_GRAPHICS_BIT.\n");
        }
    }

    void VulkanLogicalDevice::CreateLogicalDevice()
    {
        UniquePCCHElements(mEnabledLayers);
        UniquePCCHElements(mEnabledExtensions);

        Array<VkDeviceQueueCreateInfo> queueCreateInfos;
        float queuePriorities[1] = { 0.f };

        if (mSupportedQueues & VK_QUEUE_GRAPHICS_BIT)
        {
            VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
            graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            graphicsQueueCreateInfo.queueFamilyIndex = mGraphicsFamily;
            graphicsQueueCreateInfo.queueCount = 1;
            graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
            queueCreateInfos.emplace_back(graphicsQueueCreateInfo);
        }
        else
        {
            mGraphicsFamily = VK_NULL_HANDLE;
        }

        if ((mSupportedQueues & VK_QUEUE_COMPUTE_BIT) &&
            mComputeFamily != mGraphicsFamily)
        {
            VkDeviceQueueCreateInfo computeQueueCreateInfo = {};
            computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            computeQueueCreateInfo.queueFamilyIndex = mComputeFamily;
            computeQueueCreateInfo.queueCount = 1;
            computeQueueCreateInfo.pQueuePriorities = queuePriorities;
            queueCreateInfos.emplace_back(computeQueueCreateInfo);
        }
        else
        {
            mComputeFamily = mGraphicsFamily;
        }

        if ((mSupportedQueues & VK_QUEUE_TRANSFER_BIT) &&
            mTransferFamily != mGraphicsFamily)
        {
            VkDeviceQueueCreateInfo transferQueueCreateInfo = {};
            transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            transferQueueCreateInfo.queueFamilyIndex = mTransferFamily;
            transferQueueCreateInfo.queueCount = 1;
            transferQueueCreateInfo.pQueuePriorities = queuePriorities;
            queueCreateInfos.emplace_back(transferQueueCreateInfo);
        }
        else
        {
            mTransferFamily = mGraphicsFamily;
        }

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = (Uint)queueCreateInfos.size();
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        if (mVkInstance->IsValidationLayersEnabled())
        {
            deviceCreateInfo.enabledLayerCount = (Uint)mEnabledLayers.size();
            deviceCreateInfo.ppEnabledLayerNames = mEnabledLayers.data();
        }
        deviceCreateInfo.enabledExtensionCount = (Uint)mEnabledExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = mEnabledExtensions.data();
        deviceCreateInfo.pEnabledFeatures = &mEnabledFeatures;
        VkResult error = vkCreateDevice(*mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice);
        CheckVkResult(error);

        vkGetDeviceQueue(mLogicalDevice, mGraphicsFamily, 0, &mGraphicsQueue);
        vkGetDeviceQueue(mLogicalDevice, mPresentFamily, 0, &mPresentQueue);
        vkGetDeviceQueue(mLogicalDevice, mComputeFamily, 0, &mComputeQueue);
        vkGetDeviceQueue(mLogicalDevice, mTransferFamily, 0, &mTransferQueue);
    }

    void VulkanLogicalDevice::AddEnabledFeatures()
    {
        auto physicalDeviceFeatures = mPhysicalDevice->GetSupportedFeatures();

        // Enable sample rate shading filtering if supported.
        if (physicalDeviceFeatures.sampleRateShading)
        {
            mEnabledFeatures.sampleRateShading = VK_TRUE;
        }

        // Fill mode non solid is required for wireframe display.
        if (physicalDeviceFeatures.fillModeNonSolid)
        {
            mEnabledFeatures.fillModeNonSolid = VK_TRUE;

            // Wide lines must be present for line width > 1.0f.
            if (physicalDeviceFeatures.wideLines) 
            {
                mEnabledFeatures.wideLines = VK_TRUE;
            }
        }
        else 
        {
            LOG_WARNING("Selected GPU does not support wireframe pipelines!\n");
        }

        if (physicalDeviceFeatures.samplerAnisotropy) 
        {
            mEnabledFeatures.samplerAnisotropy = VK_TRUE;
        }
        else 
        {
            LOG_WARNING("Selected GPU does not support sampler anisotropy!\n");
        }

        if (physicalDeviceFeatures.textureCompressionBC) 
        {
            mEnabledFeatures.textureCompressionBC = VK_TRUE;
        }
        else if (physicalDeviceFeatures.textureCompressionASTC_LDR) 
        {
            mEnabledFeatures.textureCompressionASTC_LDR = VK_TRUE;
        }
        else if (physicalDeviceFeatures.textureCompressionETC2)
        {
            mEnabledFeatures.textureCompressionETC2 = VK_TRUE;
        }

        if (physicalDeviceFeatures.vertexPipelineStoresAndAtomics) 
        {
            mEnabledFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;
        }
        else
        {
            LOG_WARNING("Selected GPU does not support vertex pipeline stores and atomics!\n");
        }

        if (physicalDeviceFeatures.fragmentStoresAndAtomics) 
        {
            mEnabledFeatures.fragmentStoresAndAtomics = VK_TRUE;
        }
        else 
        {
            LOG_WARNING("Selected GPU does not support fragment stores and atomics!\n");
        }

        if (physicalDeviceFeatures.shaderStorageImageExtendedFormats)
        {
            mEnabledFeatures.shaderStorageImageExtendedFormats = VK_TRUE;
        }
        else 
        {
            LOG_WARNING("Selected GPU does not support shader storage extended formats!\n");
        }

        if (physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat) 
        {
            mEnabledFeatures.shaderStorageImageWriteWithoutFormat = VK_TRUE;
        }
        else
        {
            LOG_WARNING("Selected GPU does not support shader storage write without format!\n");
        }

        if (physicalDeviceFeatures.shaderClipDistance)
        {
            mEnabledFeatures.shaderClipDistance = VK_TRUE;
        }
        else
        {
            LOG_WARNING("Selected GPU does not support shader clip distance!\n");
        }

        if (physicalDeviceFeatures.shaderCullDistance)
        {
            mEnabledFeatures.shaderCullDistance = VK_TRUE;
        }
        else
        {
            LOG_WARNING("Selected GPU does not support shader cull distance!\n");
        }

        if (physicalDeviceFeatures.geometryShader) 
        {
            mEnabledFeatures.geometryShader = VK_TRUE;
        }
        else
        {
            LOG_WARNING("Selected GPU does not support geometry shaders!\n");
        }

        if (physicalDeviceFeatures.tessellationShader) 
        {
            mEnabledFeatures.tessellationShader = VK_TRUE;
        }
        else 
        {
            LOG_WARNING("Selected GPU does not support tessellation shaders!\n");
        }

        if (physicalDeviceFeatures.multiViewport)
        {
            mEnabledFeatures.multiViewport = VK_TRUE;
        }
        else 
        {
            LOG_WARNING("Selected GPU does not support multi viewports!\n");
        }
    }

    void VulkanLogicalDevice::AddEnabledLayersAndExtensions()
    {
        for (const auto& layerName : VulkanLogicalDevice::kDeviceValidationLayers)
        {
            EnableLayer(layerName);
        }

        for (const auto& extension : VulkanInstance::kInstanceExtensions)
        {
           EnableExtension(extension);
        }
    }
}