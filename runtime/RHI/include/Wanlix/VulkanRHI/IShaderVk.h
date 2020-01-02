#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/IShader.h"

namespace Wanlix
{
    class RHI_API IShaderVk : public IShader
    {
        CLASSINFO(IShaderVk)
        INHERIT_SHARED_FROM_THIS(IShader)
    public:
        virtual ByteArray const& GetSPIRV() const = 0;
    };
}