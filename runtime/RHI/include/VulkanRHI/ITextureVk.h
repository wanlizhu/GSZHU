#pragma once

#include "RHI/ITexture.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class ITextureVk : public ITexture
    {
    public:
        virtual VkImage GetVkImage() const = 0;
        virtual void SetLayout(VkImageLayout layout) = 0;
        virtual VkImageLayout GetLayout() const = 0;
    };
}