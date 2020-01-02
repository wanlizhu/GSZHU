#pragma once

#include "IDevice.h"

namespace Wanlix
{
    struct DeviceObjectDesc {};

    class RHI_API IDeviceObject : public Object
    {
        CLASSINFO(IDeviceObject)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        virtual ~IDeviceObject() = default;

        virtual Pvoid GetNativeHandle() const = 0;
        virtual const DeviceObjectDesc& GetDesc() const = 0;
        virtual IDevice* GetDevice() const = 0;
    };
}