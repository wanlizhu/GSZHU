#pragma once

#include "Wanlix/VulkanRHI/Common.h"

namespace Wanlix
{
    #ifdef WANLIX_OS_ANDROID
    // On Android we need to explicitly select all layers
    static constexpr const char* ValidationLayerNames[] =
    {
        "VK_LAYER_GOOGLE_threading",
        "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_object_tracker",
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_LUNARG_swapchain",
        "VK_LAYER_GOOGLE_unique_objects"
    };
    #else
    // On desktop the LunarG loaders exposes a meta layer that contains all layers
    static constexpr const char* ValidationLayerNames[] =
    {
        "VK_LAYER_LUNARG_standard_validation"
    };
    #endif

    void SetupVulkanDebugging(VkInstance instance,
                              VkDebugUtilsMessageSeverityFlagsEXT severity,
                              VkDebugUtilsMessageTypeFlagsEXT type,
                              void* userData = nullptr);
    void FreeVulkanDebugging(VkInstance instance);

    void SetObjectName(VkDevice device, Uint64 object, VkObjectType type, const char* name);
    void SetObjectTag(VkDevice device, Uint64 object, VkObjectType type, Uint64 name, size_t tagSize, const void* tag);

    void BeginCommandQueueLabelRegion(VkQueue queue, const char* label, Color4f color);
    void BeginCommandBufferLabelRegion(VkCommandBuffer cmdbuffer, const char* label, Color4f color);
    void InsertCommandBufferLabel(VkCommandBuffer cmdbuffer, const char* label, Color4f color);
    void EndCommandBufferLabelRegion(VkCommandBuffer cmdbuffer);
    void EndCommandQueueLabelRegion(VkQueue queue);

    const char* VkResultToString(VkResult result);
    const char* VkObjectTypeToString(VkObjectType type);
    const char* VkImageLayoutToString(VkImageLayout layout);
    std::string VkAccessFlagToString(VkAccessFlagBits bits);
}