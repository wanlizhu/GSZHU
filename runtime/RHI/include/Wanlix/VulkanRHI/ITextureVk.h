#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/ITexture.h"

namespace Wanlix
{
    class RHI_API ITextureVk : public ITexture
    {
        CLASSINFO(ITextureVk)
        INHERIT_SHARED_FROM_THIS(ITexture)
    public:
        virtual VkImage GetVkImage() const = 0;
        virtual void SetLayout(VkImageLayout layout) = 0;
        virtual VkImageLayout GetLayout() const = 0;
    };
}