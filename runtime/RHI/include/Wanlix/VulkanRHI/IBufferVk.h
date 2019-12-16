#pragma once

#include "Common.h"
#include "Wanlix/RHI/IBuffer.h"

namespace Wanlix
{
    class IBufferVk : public inherit_shared_from_this<IBuffer, IBufferVk>
    {
    public:
        virtual VkBuffer GetVkBuffer() const = 0;
        virtual void SetAccessFlags(VkAccessFlags flags) = 0;
        virtual VkAccessFlags GetAccessFlags() const = 0;
    };
}