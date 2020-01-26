#pragma once

#include "RHI/ISampler.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class ISamplerVk : public ISampler
    {
    public:
        virtual VkSampler GetVkSampler() const = 0;
    };
}