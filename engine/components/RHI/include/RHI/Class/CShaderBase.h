#pragma once

#include "RHI/IShader.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    class CShaderBase : public CDeviceObjectBase<IShader>
    {
    protected:
        CShaderBase(IDevice* device,
                    const ShaderDesc& desc,
                    const String& name)
            : CDeviceObjectBase<IShader>(device, desc, name)
        {}
    };
}