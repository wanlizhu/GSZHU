#pragma once

#include "Common.h"
#include "Wanlix/RHI/IBufferView.h"

namespace Wanlix
{
    class IBufferViewVk : public inherit_shared_from_this<IBufferView, IBufferViewVk>
    {
    public:
        virtual VkBufferView GetVkBufferView() const = 0;
    };
}