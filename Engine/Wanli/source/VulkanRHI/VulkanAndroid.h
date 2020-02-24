#if defined(__ANDROID__)
#pragma once

#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include <android/configuration.h>
#include <memory>
#include <string>
#include "VulkanFunctions.h"

#define LOG_INFO(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Vulkan", __VA_ARGS__))
#define LOG_WARNING(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Vulkan", __VA_ARGS__))
#define LOG_DEBUG(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "Vulkan", __VA_ARGS__))
#define LOG_ERROR(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "Vulkan", __VA_ARGS__))


namespace Wanli
{
    extern android_app* gpAndroidApp;
    extern int gnScreenDensity;

    bool LoadVulkanLibrary();
    void LoadVulkanFunctions();
    void FreeVulkanLibrary();
    void ShowAlert(const char* msg);
}

#endif
