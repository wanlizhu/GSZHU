#include "VulkanInstance.h"
#include "VulkanLayerAndExtension.h"
#include "VulkanPhysicalDevice.h"
#include <iostream>

namespace Wanli
{
    VkInstance VulkanInstance::globalInstance = VK_NULL_HANDLE;

    static bool _isDebugPrepared = false;
    static bool _isDebugCreated = false;
    static bool _isMessengerSupported = false;
    static VkDebugUtilsMessengerCreateInfoEXT _debugMessengerInfo = {};
    static VkDebugReportCallbackCreateInfoEXT _debugReportInfo = {};
    static VkDebugUtilsMessengerEXT _debugMessenger = nullptr;
    static VkDebugReportCallbackEXT _debugReport = nullptr;


    VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        std::cout << pCallbackData->pMessage << std::endl;
        return VK_TRUE;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(
        VkDebugReportFlagsEXT flags, 
        VkDebugReportObjectTypeEXT objectType,
        uint64_t object,
        size_t location,
        int32_t messageCode,
        const char* pLayerPrefix,
        const char* pMessage,
        void* pUserData)
    {
        std::cout << pMessage << std::endl;
        return VK_TRUE;
    }

    void DestroyDebugCallback()
    {
        if (!_isDebugCreated)
            return;

        if (_isMessengerSupported)
        {
            auto func = LOAD_FUNC(vkDestroyDebugUtilsMessengerEXT);
            if (func)
            {
                func(VulkanInstance::globalInstance, _debugMessenger, nullptr);
            }
            _debugMessenger = VK_NULL_HANDLE;
        }
        else
        {
            auto func = LOAD_FUNC(vkDestroyDebugReportCallbackEXT);
            if (func)
            {
                func(VulkanInstance::globalInstance, _debugReport, nullptr);
            }
            _debugReport = VK_NULL_HANDLE;
        }

        _isDebugCreated = false;
    }

    void PrepareDebugCallback(VkInstanceCreateInfo& instanceInfo)
    {
        if (_isDebugPrepared)
            return;

        _isMessengerSupported = VulkanLayerAndExtension::HasInstanceLayer(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        if (_isMessengerSupported)
        {
            _debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            _debugMessengerInfo.pNext = nullptr;
            _debugMessengerInfo.flags = 0;
            _debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            _debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            _debugMessengerInfo.pfnUserCallback = DebugMessengerCallback;
            _debugMessengerInfo.pUserData = nullptr;

            instanceInfo.pNext = &_debugMessengerInfo;
        }
        else
        {
            _debugReportInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
            _debugReportInfo.pNext = nullptr;
            _debugReportInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
            _debugReportInfo.pfnCallback = DebugReportCallback;
            _debugReportInfo.pUserData = nullptr;
        }

        _isDebugPrepared = true;
    }

    void CreateDebugCallback()
    {
        if (_isDebugCreated)
            return;

        if (_isMessengerSupported)
        {
            auto func = LOAD_FUNC(vkCreateDebugUtilsMessengerEXT);
            if (func)
            {
                func(VulkanInstance::globalInstance, &_debugMessengerInfo, nullptr, &_debugMessenger);
            }
        }
        else
        {
            auto func = LOAD_FUNC(vkCreateDebugReportCallbackEXT);
            if (func)
            {
                func(VulkanInstance::globalInstance, &_debugReportInfo, nullptr, &_debugReport);
            }
        }

        _isDebugCreated = true;
    }

    VulkanInstance::VulkanInstance()
    {
        VulkanLayerAndExtension::LoadFromInstanceLevel();

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = nullptr;
        appInfo.pApplicationName = VulkanConfig::appName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = VulkanConfig::engineName;
        appInfo.engineVersion = VulkanConfig::ApiVersion;

        VkInstanceCreateInfo instanceInfo = {};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pNext = nullptr;
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledExtensionCount = (uint32_t)VulkanLayerAndExtension::instanceExtensions.size();
        instanceInfo.ppEnabledExtensionNames = VulkanLayerAndExtension::instanceExtensions.data();
        if (VulkanConfig::enableValidationLayer)
        {
            instanceInfo.enabledLayerCount = (uint32_t)VulkanLayerAndExtension::instanceLayers.size();
            instanceInfo.ppEnabledLayerNames = VulkanLayerAndExtension::instanceLayers.data();
            PrepareDebugCallback(instanceInfo);
        }
        else
        {
            instanceInfo.enabledLayerCount = 0;
            instanceInfo.ppEnabledLayerNames = 0;
        }

        if (VK_SUCCEEDED(vkCreateInstance(&instanceInfo, nullptr, &instance)))
        {
            globalInstance = instance;
            CreateDebugCallback();
        }
    }

    VulkanInstance::~VulkanInstance()
    {
        if (instance)
        {
            DestroyDebugCallback();
            vkDestroyInstance(instance, nullptr);
            instance = VK_NULL_HANDLE;
        }
    }

    void* VulkanInstance::LoadFunction(const char* name)
    {
        return vkGetInstanceProcAddr(instance, name);
    }

    void* VulkanInstance::LoadFunction2(const char* name)
    {
        auto func = LoadFunction(name);
        if (func == nullptr)
        {
            throw EngineError("Failed to load function '%s'", name);
        }
        return func;
    }

    std::vector<VulkanPhysicalDevice> VulkanInstance::GetPhysicalDevices()
    {
        return {};
    }
}