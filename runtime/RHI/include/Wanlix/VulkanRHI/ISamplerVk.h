#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/ISampler.h"

namespace Wanlix
{
    class RHI_API ISamplerVk : public ISampler
    {
        CLASSINFO(ISamplerVk)
        INHERIT_SHARED_FROM_THIS(ISampler)
    public:
        virtual VkSampler GetVkSampler() const = 0;
    };
}