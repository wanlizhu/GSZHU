#pragma once

#include "GIPlatformVk.h"
#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class VKInstance : public std::enable_shared_from_this<VKInstance>
    {
        friend class GIDeviceBuilderVk;
        friend class VKInstanceBuilder;
    public:
        enum class EDebugLayerType
        {
            None,
            Messenger,
            ReportCallback,
        };

        static bool IsLayerSupported(const char* name);
        static bool IsExtensionSupported(const char* name);

        ~VKInstance();

        operator const VkInstance& () const;
        bool IsValid() const;
        bool IsValidationLayerEnabled() const;
        bool IsLayerEnabled(const char* name) const;
        bool IsExtensionEnabled(const char* name) const;

        uint32_t GetPhysicalDeviceCount() const;
        VkPhysicalDevice FirstPhysicalDevice() const;
        VkPhysicalDevice FindPhysicalDeviceBy(
            const std::vector<const char*>& extensions,
            const std::vector<const char*>& extensionsOptional,
            VkQueueFlags queueFlags,
            const VkPhysicalDeviceFeatures& features = {}) const;

    protected:
        VKInstance(
            const VkInstanceCreateInfo& createInfo,
            const std::vector<const char*>& enabledLayers,
            const std::vector<const char*>& enabledExtensions);

        VKInstance(const VKInstance&) = delete;
        VKInstance(VKInstance&&) = default;
        VkInstance& operator=(const VKInstance&) = delete;
        VKInstance& operator=(VKInstance&&) = default;

    private:
        VkInstance mInstance = VK_NULL_HANDLE;
        std::vector<const char*> mEnabledLayers; // referenced by vkCreateDevice(...)
        std::vector<const char*> mEnabledExtensions;

        union {
            VkDebugUtilsMessengerEXT messenger = VK_NULL_HANDLE;
            VkDebugReportCallbackEXT reportCallback;
        } mDebugLayer;
        EDebugLayerType mDebugLayerType = EDebugLayerType::None;
    };

    class VKInstanceBuilder
    {
    public:
        VKInstanceBuilder();
        VKInstanceBuilder& SetAppName(const char* name);
        VKInstanceBuilder& SetEngineName(const char* name);
        VKInstanceBuilder& SetApiVersion(uint32_t version);
        VKInstanceBuilder& EnableValidationLayers(bool value);
        VKInstanceBuilder& EnableLayer(const char* name, bool compulsory = false);
        VKInstanceBuilder& EnableExtension(const char* name, bool compulsory = false);
        SharedPtr<VKInstance> Build();

    private:
        VkApplicationInfo mAppInfo = {};
        VkInstanceCreateInfo mCreateInfo = {};

        std::vector<const char*> mEnabledLayers;
        std::vector<const char*> mEnabledExtensions;
        bool mEnableValidationLayers = true;
    };
}