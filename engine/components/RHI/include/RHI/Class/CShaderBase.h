#pragma once

#include "RHI/IShader.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CShaderBase : public CDeviceObjectBase<_Interface_>
    {
    protected:
        CShaderBase(IDevice* device,
                    const ShaderDesc& desc,
                    const String& name)
            : CDeviceObjectBase<_Interface_>(device, desc, name)
        {}
    };
}