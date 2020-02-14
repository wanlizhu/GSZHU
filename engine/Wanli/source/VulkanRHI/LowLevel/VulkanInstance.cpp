#include "VulkanRHI/LowLevel/VulkanInstance.h"
#include "Utilities/Log.h"
#include "Engine.h"
#include <set>
#include <algorithm>

namespace Wanli
{
    const PCCHArray VulkanInstance::kInstanceValidationLayers =
    {
#if PLATFORM_WINDOWS
    "VK_LAYER_LUNARG_standard_validation",
#elif PLATFORM_MAC
    "VK_LAYER_LUNARG_standard_validation",
    "VK_LAYER_GOOGLE_unique_objects",
    "VK_LAYER_GOOGLE_threading",
    "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_object_tracker",
#elif PLATFORM_IOS
    "MoltenVK",
#elif PLATFORM_ANDROID
    "VK_LAYER_GOOGLE_threading",
    "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_object_tracker",
    "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_LUNARG_swapchain",
    "VK_LAYER_GOOGLE_unique_objects",
#elif PLATFORM_LINUX
    "VK_LAYER_GOOGLE_threading",
    "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_object_tracker",
    "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_GOOGLE_unique_objects",
#else
#endif
    nullptr
    };

    const PCCHArray VulkanInstance::kInstanceExtensions =
    {};

    void UniquePCCHElements(PCCHArray& array)
    {
        std::sort(array.begin(), array.end(),
                  [&](const char* lhs, const char* rhs) {
                      return strcmp(lhs, rhs) < 0;
                  });
        auto it = std::unique(array.begin(), array.end());
        array.erase(it, array.end());
    }

    VulkanInstance::VulkanInstance()
    {}

    VulkanInstance::~VulkanInstance()
    {
        Destroy();
    }

    bool VulkanInstance::Initialize(bool enableDebug)
    {
        try
        {
            LoadSupportedLayersAndExtensions();
            AddEnabledPlatformExtensions();
            AddEnabledLayersAndExtensions(enableDebug);

            CreateVkInstance();
            CreateDebug(enableDebug);
            LogEnabledLayers();
        }
        catch (VulkanError & e)
        {
            LOG_ERROR("!!!Failed to initialize VulkanInstance!!!:\n");
            LOG_ERROR("\t%s\n", e.what());
            return false;
        }
        return true;
    }

    void VulkanInstance::Destroy()
    {
        mDebug.reset(nullptr);
        vkDestroyInstance(mVkInstance, nullptr);
        mVkInstance = VK_NULL_HANDLE;
    }

    void VulkanInstance::EnableExtension(const char* extension)
    {
        if (std::find_if(mSupportedExtensions.begin(), mSupportedExtensions.end(),
            [&] (const char* name) {
                return strcmp(extension, name) == 0;
            }) == mSupportedExtensions.end())
        {
            throw VulkanError("Instance extension '%s' is not present.\n", extension);
        }
        else
        {
            mEnabledExtensions.push_back(extension);
        }
    }

    void VulkanInstance::EnableLayer(const char* layerName)
    {
        VulkanInstanceLayer layer(layerName);
        mEnabledLayers.push_back(layer.GetProperties().layerName);

        for (const auto& extension : layer.GetRequiredExtensions())
        {
            mEnabledExtensions.push_back(extension.extensionName);
        }
    }

    void VulkanInstance::CreateVkInstance()
    {
        UniquePCCHElements(mEnabledLayers);
        UniquePCCHElements(mEnabledExtensions);

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = Engine::Get()->GetAppName().c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
        appInfo.pEngineName = ENGINE_NAME;
        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_1;

        VkInstanceCreateInfo instanceInfo = {};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledExtensionCount = (Uint)mEnabledExtensions.size();
        instanceInfo.ppEnabledExtensionNames = mEnabledExtensions.size() > 0 ? mEnabledExtensions.data() : nullptr;
        if (mEnableValidationLayers)
        {
            instanceInfo.enabledLayerCount = (Uint)mEnabledLayers.size();
            instanceInfo.ppEnabledLayerNames = mEnabledLayers.size() > 0 ? mEnabledLayers.data() : nullptr;
        }

        VkResult error = vkCreateInstance(&instanceInfo, nullptr, &mVkInstance);
        CheckVkResult(error);
    }

    void VulkanInstance::CreateDebug(bool enableDebug)
    {
        if (enableDebug)
        {
            mDebug = VulkanDebug::Create(mVkInstance);
        }
    }

    void VulkanInstance::LoadSupportedLayersAndExtensions()
    {
        Uint extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        Array<VkExtensionProperties> globalLayerExtension(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, globalLayerExtension.data());
        for (const auto& extension : globalLayerExtension)
        {
            mSupportedExtensions.push_back(extension.extensionName);
        }

        Uint layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        Array<VkLayerProperties> globalLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, globalLayers.data());
        for (const auto& layer : globalLayers)
        {
            mSupportedLayers.push_back(layer.layerName);
        }

        UniquePCCHElements(mSupportedLayers);
        UniquePCCHElements(mSupportedExtensions);
    }

    void VulkanInstance::AddEnabledLayersAndExtensions(bool enableDebug)
    {
        if (enableDebug)
        {
            EnableExtension(VulkanDebug::GetRequiredExtension());
        }

        for (const auto& layerName : VulkanInstance::kInstanceValidationLayers)
        {
            EnableLayer(layerName);
        }

        for (const auto& extension : VulkanInstance::kInstanceExtensions)
        {
            EnableExtension(extension);
        }
    }

    void VulkanInstance::AddEnabledPlatformExtensions()
    {
        PCCHArray extensions;
        VulkanPlatform::GetInstanceExtensions(extensions);

        for (const auto& extension : extensions)
        {
            EnableExtension(extension);
        }
    }

    void VulkanInstance::LogEnabledLayers()
    {
        LOG_INFO("Instance layers:\n");

        for (const auto& layer : mEnabledLayers)
        {
            LOG_INFO("\t%s\n", layer);
        }

        LOG_INFO("\n");
    }
}