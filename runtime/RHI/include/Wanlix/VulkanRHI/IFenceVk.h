#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/IFence.h"

namespace Wanlix
{
    class RHI_API IFenceVk : public IFence
    {
        CLASSINFO(IFenceVk)
        INHERIT_SHARED_FROM_THIS(IFence)
    };
}