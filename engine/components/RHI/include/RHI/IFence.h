#pragma once

#include "RHI/IDeviceObject.h"

namespace Wanlix
{
    struct FenceDesc : public DeviceObjectDesc
    {};

    class IFence : public IDeviceObject
    {
    public:
        using Desc = FenceDesc;

        virtual Uint64 GetCompletedValue() const = 0;
        virtual void SetValue(Uint64 value) = 0;
    };
}