#pragma once

#include "VulkanConfig.h"

namespace Wanli
{
    struct VulkanQueue
    {
        uint32_t family = 0;
        VkQueue queue = VK_NULL_HANDLE;
    };
}