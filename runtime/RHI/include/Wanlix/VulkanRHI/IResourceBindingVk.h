#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/IResourceBinding.h"

namespace Wanlix
{
    class RHI_API IResourceBindingVk : public IResourceBinding
    {
        CLASSINFO(IResourceBindingVk)
        INHERIT_SHARED_FROM_THIS(IResourceBinding)
    };
}