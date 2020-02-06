#include "VulkanRHI/VulkanRHI.h"
#include "VulkanRHI/RHIPlatform.h"
#include "VulkanRHI/RHIDevice.h"
#include "VulkanRHI/RHICommandQueue.h"
#include "VulkanRHI/RHIDeviceMemoryManager.h"
#include "VulkanRHI/RHISwapChain.h"
#include "Engine.h"
#include "Utilities/Log.h"

namespace Wanli
{
    bool VulkanRHI::Initialize(const VulkanRHICreateInfo& info)
    {
        mCreateInfo = info;
        if (!RHIPlatform::Get()->LoadVulkanLibrary())
        {
            throw RHIError("Failed to load Vulkan library");
        }

        try
        {
            CreateVkInstance();
            CreateDebugContext();
            CreateDevice();
            CreateSwapChain();
        }
        catch (RHIError& error) 
        {
            LOG_ERROR("%s\n", error.what());
            return false;
        }
        return true;
    }

    void VulkanRHI::Destroy()
    {
        if (mVkInstance != VK_NULL_HANDLE)
        {
            try 
            {
                DestroySwapChain();
                DestroyDevice();
                DestroyDebugContext();
                DestroyVkInstance();
            }
            catch (RHIError& error)
            {
                LOG_ERROR("%s\n", error.what());
            }
        }
    }

    void VulkanRHI::CreateVkInstance()
    {
        GetInstanceExtensionsAndLayers(mInstanceExtensions, mInstanceLayers);

        if (!mCreateInfo.requiredInstanceExtensions.empty())
        {
            LOG_DEBUG("Using app instance extensions:\n");
            for (const auto& name : mCreateInfo.requiredInstanceExtensions)
            {
                mInstanceExtensions.emplace_back(name);
                LOG_DEBUG("\t* %s.\n", name);
            }
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = Engine::Get()->GetApplication()->GetTitle();
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
        appInfo.pEngineName = ENGINE_NAME;
        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
        appInfo.apiVersion = VK_MAKE_VERSION(mCreateInfo.version.major, mCreateInfo.version.minor, mCreateInfo.version.patch);

        VkInstanceCreateInfo instanceInfo = {};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledExtensionCount = (uint32_t)mInstanceExtensions.size();
        instanceInfo.ppEnabledExtensionNames = mInstanceExtensions.size() > 0 ? mInstanceExtensions.data() : nullptr;
        instanceInfo.enabledLayerCount = (uint32_t)mInstanceLayers.size();
        instanceInfo.ppEnabledLayerNames = mInstanceLayers.size() > 0 ? mInstanceLayers.data() : nullptr;
    
        VkResult result = vkCreateInstance(&instanceInfo, VK_CPU_ALLOCATOR, &mVkInstance);

        if (result == VK_ERROR_INCOMPATIBLE_DRIVER)
        {
            LOG_ERROR("Can't find a compatible Vulkan driver (ICD)");
        }
        else if (result == VK_ERROR_EXTENSION_NOT_PRESENT)
        {
            LOG_ERROR("Missing required instance extensions:\n");

            uint32_t propertyCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
            std::vector<VkExtensionProperties> properties(propertyCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties.data());

            for (const char* extension : mInstanceExtensions)
            {
                if (std::find_if(properties.begin(), properties.end(),
                    [&] (const VkExtensionProperties& prop) {
                        return strcmp(prop.extensionName, extension) == 0;
                    }) == properties.end())
                {
                    LOG_ERROR("\t* %s.\n", extension);
                }
            }
        }
        else if (result != VK_SUCCESS)
        {
            LOG_ERROR("Failed to create Vulkan instance.\n");
        }
        else 
        {
            LOG_DEBUG("Create Vulkan instance success.\n");
        }
    }

    void VulkanRHI::CreateDebugContext()
    {

    }

    void VulkanRHI::CreateDevice()
    {

    }

    void VulkanRHI::CreateSwapChain()
    {

    }

    void VulkanRHI::DestroySwapChain()
    {

    }

    void VulkanRHI::DestroyDevice()
    {

    }

    void VulkanRHI::DestroyDebugContext()
    {

    }

    void VulkanRHI::DestroyVkInstance()
    {

    }

    void VulkanRHI::GetInstanceExtensionsAndLayers(PCCHArray& extensions, PCCHArray& layers)
    {

    }

    Uint VulkanRHI::SelectPhysicalDevice()
    {

    }

}