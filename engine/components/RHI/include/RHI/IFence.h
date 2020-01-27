#pragma once

#include "RHI/IDeviceObject.h"

namespace Wanlix
{
    struct FenceDesc : public DeviceObjectDesc
    {};

    class IFence : public IDeviceObject
    {
    public:
        virtual Uint64 GetCompletedValue() const = 0;
        virtual void SetValue(Uint64 value) = 0;
    };
}