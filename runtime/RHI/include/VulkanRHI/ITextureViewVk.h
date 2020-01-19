#pragma once

#include "RHI/ISwapChain.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class ITextureViewVk : public ITextureView
    {
    public:
        virtual VkImageView GetVkImageView() const = 0;
    };
}