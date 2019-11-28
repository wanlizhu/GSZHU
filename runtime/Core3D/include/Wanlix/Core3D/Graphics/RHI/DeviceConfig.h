#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace Wanlix
{
    enum class OpenGLContextProfile
    {
        CompatibilityProfile,
        CoreProfile,
        ESProfile,
    };

    /* Only supported with: Vulkan. */
    struct ApplicationDescriptor
    {
        std::string applicationName;
        uint32_t    applicationVersion;
        std::string engineName;
        uint32_t    engineVersion;
    };

    struct VulkanDeviceConfig
    {
        ApplicationDescriptor    application;
        std::vector<std::string> enabledLayers;
        std::uint64_t            minDeviceMemoryAllocationSize = 1024 * 1024;
        bool                     reduceDeviceMemoryFragmentation = false;
    };

    struct OpenGLDeviceConfig
    {
        OpenGLContextProfile contextProfile = OpenGLContextProfile::CoreProfile;
        int                  majorVersion = 0;
        int                  minorVersion = 0;
    };

}