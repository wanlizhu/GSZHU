#include "VulkanRHI/LowLevel/VulkanDebug.h"
#include "Utilities/Log.h"
#include "VulkanRHI/VulkanError.h"

namespace Wanli
{
    UniquePtr<VulkanDebug> VulkanDebug::Create(VkInstance instance)
    {
#if USE_DEBUG_MESSENGER
            return std::make_unique<VulkanDebugMessenger>(instance);
#else
            return std::make_unique<VulkanDebugReport>(instance);
#endif
    }

    const char* VulkanDebug::GetRequiredExtension()
    {
#if USE_DEBUG_MESSENGER
            return VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
#else
            return VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
#endif
    }

    VulkanDebug::VulkanDebug(VkInstance instance)
        : mVkInstance(instance)
    {}

#if USE_DEBUG_MESSENGER
    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugMessenger::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    )
    {
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            LOG_ERROR("%s\n", pCallbackData->pMessage);
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            LOG_WARNING("%s\n", pCallbackData->pMessage);
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        {
            LOG_INFO("%s\n", pCallbackData->pMessage);
        }
        return VK_FALSE;
    }

    VulkanDebugMessenger::VulkanDebugMessenger(VkInstance instance)
        : VulkanDebug(instance)
    {}

    VulkanDebugMessenger::~VulkanDebugMessenger()
    {
        Destroy();
    }

    bool VulkanDebugMessenger::Initialize()
    {
        VkDebugUtilsMessengerCreateInfoEXT info = {};
        info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        info.pfnUserCallback = &VulkanDebugMessenger::DebugCallback;

        auto createMethod = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mVkInstance, "vkCreateDebugUtilsMessengerEXT");
        VkResult error = VK_ERROR_EXTENSION_NOT_PRESENT;
        if (createMethod)
        {
            error = createMethod(mVkInstance, &info, nullptr, &mMessenger);
        }

        if (error == VK_ERROR_EXTENSION_NOT_PRESENT)
        {
            LOG_ERROR("Extension vkCreateDebugUtilsMessengerEXT is not present!\n");
        }
        else
        {
            CheckVkResult(error);
        }

        return error == VK_SUCCESS;
    }

    void VulkanDebugMessenger::Destroy()
    {
        if (mMessenger != VK_NULL_HANDLE)
        {
            auto destroyMethod = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mVkInstance, "vkDestroyDebugUtilsMessengerEXT");
            destroyMethod(mVkInstance, mMessenger, nullptr);
            mMessenger = VK_NULL_HANDLE;
            mVkInstance = VK_NULL_HANDLE;
}
    }
#else
    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReport::DebugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objectType,
        uint64_t object,
        size_t location,
        int32_t messageCode,
        const char* pLayerPrefix,
        const char* pMessage,
        void* pUserData
    )
    {
        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
        {
            LOG_ERROR("%s\n", pMessage);
        }
        else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
        {
            LOG_WARNING("%s\n", pMessage);
        }
        else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
        {
            LOG_WARNING("%s\n", pMessage);
        }
        else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
        {
            LOG_DEBUG("%s\n", pMessage);
        }
        else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
        {
            LOG_INFO("%s\n", pMessage);
        }
        return VK_FALSE;
    }

    VulkanDebugReport::VulkanDebugReport(VkInstance instance)
        : VulkanDebug(instance)
    {}

    VulkanDebugReport::~VulkanDebugReport()
    {
        Destroy();
    }

    bool VulkanDebugReport::Initialize()
    {
        VkDebugReportCallbackCreateInfoEXT info = {};
        info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        info.pNext = nullptr;
        info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | 
            VK_DEBUG_REPORT_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_DEBUG_BIT_EXT |
            VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
        info.pfnCallback = &VulkanDebugReport::DebugCallback;
        info.pUserData = this;
        
        auto createMethod = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(mVkInstance, "vkCreateDebugReportCallbackEXT");
        VkResult error = VK_ERROR_EXTENSION_NOT_PRESENT;
        if (createMethod)
        {
            error = createMethod(mVkInstance, &info, nullptr, &mReportCallback);
        }

        if (error == VK_ERROR_EXTENSION_NOT_PRESENT)
        {
            LOG_ERROR("Extension vkCreateDebugReportCallbackEXT is not present!\n");
        }
        else
        {
            CheckVkResult(error);
        }
        
        return error == VK_SUCCESS;
    }

    void VulkanDebugReport::Destroy()
    {
        if (mReportCallback != VK_NULL_HANDLE)
        {
            auto destroyMethod = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(mVkInstance, "vkDestroyDebugReportCallbackEXT");
            destroyMethod(mVkInstance, mReportCallback, nullptr);
            mReportCallback = VK_NULL_HANDLE;
            mVkInstance = VK_NULL_HANDLE;
        }
    }
#endif

    void CheckVkResult(VkResult result)
    {
        switch (result)
        {
            case VK_ERROR_OUT_OF_HOST_MEMORY: throw VulkanError("VK_ERROR_OUT_OF_HOST_MEMORY");
            case VK_ERROR_OUT_OF_DEVICE_MEMORY: throw VulkanError("VK_ERROR_OUT_OF_DEVICE_MEMORY");
            case VK_ERROR_INITIALIZATION_FAILED: throw VulkanError("VK_ERROR_INITIALIZATION_FAILED");
            case VK_ERROR_DEVICE_LOST: throw VulkanError("VK_ERROR_DEVICE_LOST");
            case VK_ERROR_MEMORY_MAP_FAILED: throw VulkanError("VK_ERROR_MEMORY_MAP_FAILED");
            case VK_ERROR_LAYER_NOT_PRESENT: throw VulkanError("VK_ERROR_LAYER_NOT_PRESENT");
            case VK_ERROR_EXTENSION_NOT_PRESENT: throw VulkanError("VK_ERROR_EXTENSION_NOT_PRESENT");
            case VK_ERROR_FEATURE_NOT_PRESENT: throw VulkanError("VK_ERROR_FEATURE_NOT_PRESENT");
            case VK_ERROR_INCOMPATIBLE_DRIVER: throw VulkanError("VK_ERROR_INCOMPATIBLE_DRIVER");
            case VK_ERROR_TOO_MANY_OBJECTS: throw VulkanError("VK_ERROR_TOO_MANY_OBJECTS");
            case VK_ERROR_FORMAT_NOT_SUPPORTED: throw VulkanError("VK_ERROR_FORMAT_NOT_SUPPORTED");
            case VK_ERROR_FRAGMENTED_POOL: throw VulkanError("VK_ERROR_FRAGMENTED_POOL");
            case VK_ERROR_OUT_OF_POOL_MEMORY: throw VulkanError("VK_ERROR_OUT_OF_POOL_MEMORY");
            case VK_ERROR_INVALID_EXTERNAL_HANDLE: throw VulkanError("VK_ERROR_INVALID_EXTERNAL_HANDLE");
            case VK_ERROR_SURFACE_LOST_KHR: throw VulkanError("VK_ERROR_SURFACE_LOST_KHR");
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: throw VulkanError("VK_ERROR_NATIVE_WINDOW_IN_USE_KHR");
            case VK_SUBOPTIMAL_KHR: throw VulkanError("VK_SUBOPTIMAL_KHR");
            case VK_ERROR_OUT_OF_DATE_KHR: throw VulkanError("VK_ERROR_OUT_OF_DATE_KHR");
            case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: throw VulkanError("VK_ERROR_INCOMPATIBLE_DISPLAY_KHR");
            case VK_ERROR_VALIDATION_FAILED_EXT: throw VulkanError("VK_ERROR_VALIDATION_FAILED_EXT");
            case VK_ERROR_INVALID_SHADER_NV: throw VulkanError("VK_ERROR_INVALID_SHADER_NV");
            case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: throw VulkanError("VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT");
            case VK_ERROR_FRAGMENTATION_EXT: throw VulkanError("VK_ERROR_FRAGMENTATION_EXT");
            case VK_ERROR_NOT_PERMITTED_EXT: throw VulkanError("VK_ERROR_NOT_PERMITTED_EXT");
            case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT: throw VulkanError("VK_ERROR_INVALID_DEVICE_ADDRESS_EXT");
            case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: throw VulkanError("VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT");
            default:break;
        }
    }
}