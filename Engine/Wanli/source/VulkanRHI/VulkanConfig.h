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

#define VK_SUCCEEDED(code) VkResult _error = code; _error == VK_SUCCESS 
#define VK_FAILED(code) VkResult _error = code; _error != VK_SUCCESS
#define VK_THROW_LAST_ERROR() if (_error != VK_SUCCESS) { throw VulkanError(_error); }

namespace Wanli
{
    class VulkanError : public std::runtime_error
    {
    public:
        VulkanError(VkResult code)
            : std::runtime_error(std::to_string((int)code))
        {}
    };

    class EngineError : public std::runtime_error
    {
        char buffer[1024];
    public:
        template<typename... Args>
        EngineError(const char* format, Args... args)
            : std::runtime_error((snprintf(buffer, 1024, format, args...), buffer))
        {}
    };

    struct VulkanConfig
    {
        static const char* AppName;
        static const char* EngineName;
        static uint32_t APIVersion;

        static bool Vsync;
        static bool UseVMA;
        static bool EnableValidationLayer;
        static int BackbufferCount;

        static std::vector<const char*> RequiredInstanceExtensions;
        static std::vector<const char*> RequiredInstanceLayers;
        static std::vector<const char*> RequiredDeviceExtensions;
        static std::vector<const char*> RequiredDeviceLayers;
    };
}