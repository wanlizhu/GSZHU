#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/ITextureView.h"

namespace Wanlix
{
    class RHI_API ITextureViewVk : public ITextureView
    {
        CLASSINFO(ITextureViewVk)
        INHERIT_SHARED_FROM_THIS(ITextureView)
    public:
        virtual VkImageView GetVkImageView() const = 0;
    };
}