#pragma once

#include "RHI/ISwapChain.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class ISwapChainVk : public ISwapChain
    {
    public:
        virtual VkSwapchainKHR GetVkSwapChain() const = 0;
    };
}