#include "Graphics/VulkanRHI/RHIDebugContext.h"
#include "Utilities/Logger.h"

#define VK_CREATE_FUNC_NAME "vkCreateDebugReportCallbackEXT"
#define VK_DESTROY_FUNC_NAME "vkDestroyDebugReportCallbackEXT"

namespace Wanli
{
    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t obj,
        size_t location,
        int32_t code,
        const char* layerPrefix,
        const char* msg,
        void* userData
    )
    {
        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
        {
            LOG_ERROR("ERROR: [%s] Code %d : %s", layerPrefix, code, msg);
        }

        if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
        {
            LOG_WARNING("WARNING: [%s] Code %d : %s", layerPrefix, code, msg);
        }

        if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
        {
            LOG_INFO("PERFORMANCE: [%s] Code %d : %s", layerPrefix, code, msg);
        }

        if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
        {
            LOG_INFO("INFO: [%s] Code %d : %s", layerPrefix, code, msg);
        }

        if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
        {
            LOG_DEBUG("DEBUG: [%s] Code %d : %s", layerPrefix, code, msg);
        }

        return VK_FALSE;
    }

    RHIDebugContext::RHIDebugContext(VkInstance instance)
        : mVkInstance(instance)
    {}

    RHIDebugContext::~RHIDebugContext()
    {
        Destroy();
    }

    void RHIDebugContext::Initialize(EReportType reportTypes)
    {
        if (mVkInstance == VK_NULL_HANDLE)
        {
            return;
        }

        VkDebugReportCallbackCreateInfoEXT debugInfo = {};
        debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        debugInfo.flags = ToDebugReportFlags(reportTypes);
        debugInfo.pfnCallback = VulkanDebugCallback;
        debugInfo.pUserData = this;

        auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(mVkInstance, VK_CREATE_FUNC_NAME);
        if (func == nullptr)
        {
            LOG_ERROR("Failed to load function pointer: '%s'.\n", VK_CREATE_FUNC_NAME);
            return;
        }

        if (!func(mVkInstance, &debugInfo, nullptr, &mDebugCallback))
        {
            LOG_ERROR("Failed to establish debug report callback.\n");
            return;
        }

        LOG_DEBUG("Establish debug report callback success.\n");
    }

    void RHIDebugContext::Destroy()
    {
        if (mDebugCallback != VK_NULL_HANDLE)
        {
            auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(mVkInstance, VK_DESTROY_FUNC_NAME);
            if (func)
            {
                func(mVkInstance, mDebugCallback, nullptr);
            }
            mVkInstance = VK_NULL_HANDLE;
            mDebugCallback = VK_NULL_HANDLE;
        }
    }

    bool RHIDebugContext::IsEnabled() const
    {
        return mDebugCallback != VK_NULL_HANDLE;
    }

    VkDebugReportFlagsEXT RHIDebugContext::ToDebugReportFlags(RHIDebugContext::EReportType types)
    {
        VkDebugReportFlagsEXT flags = 0;

        if (types & RT_Error)
        {
            flags |= VK_DEBUG_REPORT_ERROR_BIT_EXT;
        }

        if (types & RT_Warning)
        {
            flags |= VK_DEBUG_REPORT_WARNING_BIT_EXT;
        }

        if (types & RT_PerformanceWarning)
        {
            flags |= VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        }

        if (types & RT_Information)
        {
            flags |= VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
        }

        if (types & RT_Debug)
        {
            flags |= VK_DEBUG_REPORT_DEBUG_BIT_EXT;
        }

        return flags;
    }
}