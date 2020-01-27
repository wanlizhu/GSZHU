#pragma once

#include "RHI/IBuffer.h"
#include "VulkanRHI/VkUtilities.h"
#include "VulkanRHI/IDeviceVk.h"

namespace Wanlix
{
    class IBufferVk : public IBuffer
    {
    public:
        virtual VkBuffer GetVkBuffer() const = 0;
        virtual void SetAccessFlags(VkAccessFlags flags) = 0;
        virtual VkAccessFlags GetAccessFlags() const = 0;
    };
}