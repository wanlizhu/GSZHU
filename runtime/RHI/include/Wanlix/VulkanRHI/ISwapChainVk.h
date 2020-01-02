#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/ISwapChain.h"

namespace Wanlix
{
    class RHI_API ISwapChainVk : public ISwapChain
    {
        CLASSINFO(ISwapChainVk)
        INHERIT_SHARED_FROM_THIS(ISwapChain)
    public:
        virtual VkSwapchainKHR GetVkSwapChain() const = 0;
    };
}