#include "VulkanLogicalDevice.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSurface.h"
#include <optional>
#include <vector>

namespace Wanli
{
    static void FindQueueFamilies(VulkanLogicalDevice* device);
    static void PreparePhysicalDeviceFeatures(VulkanLogicalDevice* device);
    static void PrepareDeviceQueues(VulkanLogicalDevice* device, std::vector<VkDeviceQueueCreateInfo>* infos);

    VulkanLogicalDevice::VulkanLogicalDevice(
        VulkanInstance* instance,
        VulkanPhysicalDevice* physicalDevice,
        VulkanSurface* surface)
        : instance(instance)
        , physicalDevice(physicalDevice)
        , surface(surface)
        , logicalDevice(VK_NULL_HANDLE)
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

        FindQueueFamilies(this);
        PreparePhysicalDeviceFeatures(this);
        PrepareDeviceQueues(this, &queueCreateInfos);

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = (uint32_t)(queueCreateInfos.size());
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        if (VulkanConfig::enableValidationLayer)
        {
            deviceCreateInfo.enabledLayerCount = (uint32_t)(VulkanConfig::requiredDeviceLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = VulkanConfig::requiredDeviceLayers.data();
        }
        deviceCreateInfo.enabledExtensionCount = (uint32_t)(VulkanConfig::requiredDeviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = VulkanConfig::requiredDeviceExtensions.data();
        deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

        VK_CHECK(vkCreateDevice(*physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice));

        vkGetDeviceQueue(logicalDevice, graphicsQueue.family, 0, &(graphicsQueue.queue));
        vkGetDeviceQueue(logicalDevice, presentQueue.family, 0, &(presentQueue.queue));
        vkGetDeviceQueue(logicalDevice, computeQueue.family, 0, &(computeQueue.queue));
        vkGetDeviceQueue(logicalDevice, transferQueue.family, 0, &(transferQueue.queue));
    }

    VulkanLogicalDevice::~VulkanLogicalDevice()
    {
        if (logicalDevice)
        {
            vkDeviceWaitIdle(logicalDevice);
            vkDestroyDevice(logicalDevice, nullptr);
            logicalDevice = VK_NULL_HANDLE;
        }
    }

    static void FindQueueFamilies(VulkanLogicalDevice* device)
    {
        uint32_t familyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(*(device->physicalDevice), &familyCount, nullptr);
        std::vector<VkQueueFamilyProperties> familyProps(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(*(device->physicalDevice), &familyCount, familyProps.data());

        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        std::optional<uint32_t> computeFamily;
        std::optional<uint32_t> transferFamily;

        for (uint32_t i = 0; i < familyCount; i++)
        {
            // Check for graphics support.
            if (familyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                graphicsFamily = i;
                device->graphicsQueue.family = i;
                device->supportedQueues |= VK_QUEUE_GRAPHICS_BIT;
            }

            // Check for presentation support.
            VkBool32 presentSupport;
            vkGetPhysicalDeviceSurfaceSupportKHR(*(device->physicalDevice), i, *(device->surface), &presentSupport);
            if (presentSupport && familyProps[i].queueCount > 0)
            {
                presentFamily = i;
                device->presentQueue.family = i;
            }

            // Check for compute support.
            if (familyProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                computeFamily = i;
                device->computeQueue.family = i;
                device->supportedQueues |= VK_QUEUE_COMPUTE_BIT;
            }

            // Check for transfer support.
            if (familyProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                transferFamily = i;
                device->transferQueue.family = i;
                device->supportedQueues |= VK_QUEUE_TRANSFER_BIT;
            }

            // Adequate pre-create preparation.
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
            throw std::runtime_error("No device queue supports VK_QUEUE_GRAPHICS_BIT");
        }
    }

    static void PreparePhysicalDeviceFeatures(VulkanLogicalDevice* device)
    {
        const auto& supportedFeatures = device->physicalDevice->supportedFeatures;

        if (supportedFeatures.sampleRateShading)
            device->enabledFeatures.sampleRateShading = VK_TRUE;

        if (supportedFeatures.fillModeNonSolid)
            device->enabledFeatures.fillModeNonSolid = VK_TRUE;

        if (supportedFeatures.wideLines)
            device->enabledFeatures.wideLines = VK_TRUE;

        if (supportedFeatures.samplerAnisotropy)
            device->enabledFeatures.samplerAnisotropy = VK_TRUE;

        if (supportedFeatures.vertexPipelineStoresAndAtomics)
            device->enabledFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;

        if (supportedFeatures.fragmentStoresAndAtomics)
            device->enabledFeatures.fragmentStoresAndAtomics = VK_TRUE;

        if (supportedFeatures.shaderStorageImageExtendedFormats)
            device->enabledFeatures.shaderStorageImageExtendedFormats = VK_TRUE;

        if (supportedFeatures.shaderStorageImageWriteWithoutFormat)
            device->enabledFeatures.shaderStorageImageWriteWithoutFormat = VK_TRUE;

        if (supportedFeatures.geometryShader)
            device->enabledFeatures.geometryShader = VK_TRUE;

        if (supportedFeatures.tessellationShader)
            device->enabledFeatures.tessellationShader = VK_TRUE;

        if (supportedFeatures.multiViewport)
            device->enabledFeatures.multiViewport = VK_TRUE;

        if (supportedFeatures.textureCompressionBC)
            device->enabledFeatures.textureCompressionBC = VK_TRUE;
        else if (supportedFeatures.textureCompressionASTC_LDR)
            device->enabledFeatures.textureCompressionASTC_LDR = VK_TRUE;
        else if (supportedFeatures.textureCompressionETC2)
            device->enabledFeatures.textureCompressionETC2 = VK_TRUE;
    }

    static void PrepareDeviceQueues(VulkanLogicalDevice* device, std::vector<VkDeviceQueueCreateInfo>* infos)
    {
        std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos = *infos;
        float queuePriorities[1] = { 0.f };

        if (device->supportedQueues & VK_QUEUE_GRAPHICS_BIT)
        {
            VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
            graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            graphicsQueueCreateInfo.queueFamilyIndex = device->graphicsQueue.family;
            graphicsQueueCreateInfo.queueCount = 1;
            graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
            queueCreateInfos.emplace_back(graphicsQueueCreateInfo);
        }
        else
        {
            device->graphicsQueue.family = 0;
        }

        if ((device->supportedQueues & VK_QUEUE_COMPUTE_BIT) &&
            (device->graphicsQueue.family != device->computeQueue.family))
        {
            VkDeviceQueueCreateInfo computeQueueCreateInfo = {};
            computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            computeQueueCreateInfo.queueFamilyIndex = device->computeQueue.family;
            computeQueueCreateInfo.queueCount = 1;
            computeQueueCreateInfo.pQueuePriorities = queuePriorities;
            queueCreateInfos.emplace_back(computeQueueCreateInfo);
        }
        else
        {
            device->computeQueue.family = device->graphicsQueue.family;
        }

        if ((device->supportedQueues & VK_QUEUE_TRANSFER_BIT) &&
            (device->transferQueue.family != device->graphicsQueue.family) &&
            (device->transferQueue.family != device->computeQueue.family))
        {
            VkDeviceQueueCreateInfo transferQueueCreateInfo = {};
            transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            transferQueueCreateInfo.queueFamilyIndex = device->transferQueue.family;
            transferQueueCreateInfo.queueCount = 1;
            transferQueueCreateInfo.pQueuePriorities = queuePriorities;
            queueCreateInfos.emplace_back(transferQueueCreateInfo);
        }
        else
        {
            device->transferQueue.family = device->graphicsQueue.family;
        }
    }
}