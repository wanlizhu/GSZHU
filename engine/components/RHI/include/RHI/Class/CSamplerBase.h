#pragma once

#include "RHI/ISampler.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    class CSamplerBase : public CDeviceObjectBase<ISampler>
    {
    protected:
        CSamplerBase(IDevice* device,
                     const SamplerDesc& desc,
                     const String& name)
            : CDeviceObjectBase<ISampler>(device, desc, name)
        {}
    };
}