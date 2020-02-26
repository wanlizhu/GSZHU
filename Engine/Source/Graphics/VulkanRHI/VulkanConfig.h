#pragma once

#if defined _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#endif

#ifdef ANDROID
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/configuration.h>
#include <memory>
#include <string>
#include <VulkanAndroid.h>
#endif

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <exception>

#define VK_SUCCEEDED(error) error == VK_SUCCESS 
#define VK_FAILED(error) error != VK_SUCCESS
#define VK_CHECK(error) if (error != VK_SUCCESS) { throw VulkanError(error); }

namespace Wanli
{
    class VulkanError : public std::runtime_error
    {
    public:
        static std::string ToString(VkResult error);

        VulkanError(VkResult error)
            : std::runtime_error("Vulkan Error: " + VulkanError::ToString(error))
        {}
    };

    struct VulkanConfig
    {
        static const char* appName;
        static const char* engineName;
        static uint32_t ApiVersion;

        static bool useVMA;
        static bool enableVsync;
        static bool enableAsyncSubmit;
        static bool enableValidationLayer;
        static int  backbufferCount;

        static std::vector<const char*> requiredInstanceExtensions;
        static std::vector<const char*> requiredInstanceLayers;
        static std::vector<const char*> requiredDeviceExtensions;
        static std::vector<const char*> requiredDeviceLayers;
    };
}