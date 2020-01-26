#pragma once

#include "RHI/ICommandQueue.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    class CCommandQueueBase : public CDeviceObjectBase<ICommandQueue>
    {
    protected:
        CCommandQueueBase(IDevice* device,
                          const String& name,
                          Uint familyIndex)
            : CDeviceObjectBase<ICommandQueue>(device, CommandQueueDesc(), name)
            , mQueueFamilyIndex(familyIndex)
        {}

    protected:
        Uint mQueueFamilyIndex = 0;
    };
}