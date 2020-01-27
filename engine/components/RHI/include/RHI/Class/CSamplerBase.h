#pragma once

#include "RHI/ISampler.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CSamplerBase : public CDeviceObjectBase<_Interface_>
    {
    protected:
        CSamplerBase(IDevice* device,
                     const SamplerDesc& desc,
                     const String& name)
            : CDeviceObjectBase<_Interface_>(device, desc, name)
        {}
    };
}