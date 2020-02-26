#ifdef PLATFORM_ANDROID
#include "VulkanConfig.h"

namespace Wanli
{
    std::vector<const char*> VulkanConfig::requiredInstanceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME,
        "VK_KHR_maintenance1",
#ifdef _DEBUG
        "VK_EXT_debug_report",
#endif
    };

    std::vector<const char*> VulkanConfig::requiredInstanceLayers = 
    {
        "VK_LAYER_GOOGLE_threading",
        "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_object_tracker",
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_LUNARG_swapchain",
        "VK_LAYER_GOOGLE_unique_objects",
    };

    std::vector<const char*> VulkanConfig::requiredDeviceExtensions = {};

    std::vector<const char*> VulkanConfig::requiredDeviceLayers = {};
}
#endif