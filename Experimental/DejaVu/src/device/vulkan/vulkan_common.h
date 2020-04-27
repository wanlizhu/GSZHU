#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <vulkan/vulkan.h>
#include <json/json.h>
#include "vulkan_handle.h"

#define VK_CHECK_RESULT(expr) \
{ \
    VkResult result = (expr); \
    if (result != VK_SUCCESS) \
    { \
        std::cerr << "Error: " << result << ": " << to_string(result) \
                  << " in " << __FILE__ << " at line " << __LINE__ << std::endl; \
        assert(result == VK_SUCCESS); \
    } \
}


namespace djv
{
    std::string to_string(VkResult error);
    std::string to_string(VkPhysicalDeviceType type);
    VkFormat select_depth_format(VkPhysicalDevice device);
    VulkanHandle<VkShaderModule> load_shader_module(VkDevice device, const std::string& path, Json::Value* reflection);
}