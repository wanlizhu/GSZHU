#ifdef PLATFORM_ANDROID
#include "VulkanConfig.h"

namespace Wanli
{
    const char* VulkanConfig::AppName = "Unnamed";
    const char* VulkanConfig::EngineName = "Wanli";
    uint32_t VulkanConfig::APIVersion = VK_API_VERSION_1_2;

    bool VulkanConfig::Vsync = true;
    bool VulkanConfig::UseVMA = true;
    bool VulkanConfig::EnableValidationLayer = (bool)_DEBUG;
    int VulkanConfig::BackbufferCount = 2;

    std::vector<const char*> VulkanConfig::RequiredInstanceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME,
        "VK_KHR_maintenance1",
#ifdef _DEBUG
        "VK_EXT_debug_report",
#endif
    };

    std::vector<const char*> VulkanConfig::RequiredInstanceLayers = 
    {
        "VK_LAYER_GOOGLE_threading",
        "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_object_tracker",
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_LUNARG_swapchain",
        "VK_LAYER_GOOGLE_unique_objects",
    };

    std::vector<const char*> VulkanConfig::RequiredDeviceExtensions = {};

    std::vector<const char*> VulkanConfig::RequiredDeviceLayers = {};
}
#endif