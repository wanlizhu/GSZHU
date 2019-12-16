#pragma once

#include "IDeviceObject.h"

namespace Wanlix
{
    struct FenceDesc : public DeviceObjectDesc
    {};

    class IFence : public inherit_shared_from_this<IDeviceObject, IFence>
    {
    public:
        virtual ~IFence() = default;

        /* interfaces inherited from IDeviceObject */
        virtual Pvoid GetNativeHandle() const = 0;
        virtual FenceDesc const& GetDesc() const override = 0;
        virtual IDevice* GetDevice() const override = 0;

        virtual Ui64 GetCompletedValue() const = 0;
        virtual void Reset(Ui64 value) = 0;
    };
}