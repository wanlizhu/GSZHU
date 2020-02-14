#pragma once

#include "VulkanRHI/VulkanConfig.h"
#include "VulkanRHI/VulkanError.h"

#ifdef VK_EXT_debug_utils
#define USE_DEBUG_MESSENGER true
#else 
#define USE_DEBUG_MESSENGER false
#endif

namespace Wanli
{
    enum class EDebugLevel
    {
        Disabled,
        Info,
        Debug,
        Warning,
        Error,

        Max = Error,
        Min = Info,
    };

    class DLLDECL VulkanDebug : public NonCopyable
    {
    public:
        static UniquePtr<VulkanDebug> Create(VkInstance instance);
        static const char* GetRequiredExtension();

        VulkanDebug(VkInstance instance);
        virtual ~VulkanDebug() = default;

        virtual bool Initialize() = 0;
        virtual void Destroy() = 0;
        inline void SetDebugLevel(EDebugLevel level) { mDebugLevel = level; }

    protected:
        VkInstance mVkInstance = VK_NULL_HANDLE;
        EDebugLevel mDebugLevel = EDebugLevel::Max;
    };

#if USE_DEBUG_MESSENGER
    class DLLDECL VulkanDebugMessenger final : public VulkanDebug
    {
    public:
        VulkanDebugMessenger(VkInstance instance);
        virtual ~VulkanDebugMessenger();

        virtual bool Initialize() override final;
        virtual void Destroy() override final;

    private:
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
        );

    private:
        VkDebugUtilsMessengerEXT mMessenger = VK_NULL_HANDLE;
    };
#else
    class DLLDECL VulkanDebugReport final : public VulkanDebug
    {
    public:
        VulkanDebugReport(VkInstance instance);
        virtual ~VulkanDebugReport();

        virtual bool Initialize() override final;
        virtual void Destroy() override final;

    private:
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugReportFlagsEXT flags,
            VkDebugReportObjectTypeEXT objectType,
            uint64_t object,
            size_t location,
            int32_t messageCode,
            const char* pLayerPrefix,
            const char* pMessage,
            void* pUserData
        );

    private:
        VkDebugReportCallbackEXT mReportCallback = VK_NULL_HANDLE;
    };
#endif

    void CheckVkResult(VkResult result);
}