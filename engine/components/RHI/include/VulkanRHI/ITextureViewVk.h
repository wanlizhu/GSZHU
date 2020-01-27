#pragma once

#include "RHI/ISwapChain.h"
#include "VulkanRHI/VkUtilities.h"
#include "VulkanRHI/IDeviceVk.h"

namespace Wanlix
{
    class ITextureViewVk : public ITextureView
    {
    public:
        virtual VkImageView GetVkImageView() const = 0;
    };
}