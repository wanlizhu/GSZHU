#pragma once

#include "IDevice.h"

namespace Wanlix
{
    struct DeviceObjectDesc {};

    class IDeviceObject : public inherit_shared_from_this<Object, IDeviceObject>
    {
    public:
        virtual ~IDeviceObject() = default;

        virtual Pvoid GetNativeHandle() const = 0;
        virtual const DeviceObjectDesc& GetDesc() const = 0;
        virtual IDevice* GetDevice() const = 0;
    };
}