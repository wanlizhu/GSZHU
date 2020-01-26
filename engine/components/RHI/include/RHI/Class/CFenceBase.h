#pragma once

#include "RHI/IFence.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    class CFenceBase : public CDeviceObjectBase<IFence>
    {
    protected:
        CFenceBase(IDevice* device, const String& name)
            : CDeviceObjectBase<IFence>(device, FenceDesc(), name)
        {}
    };
}