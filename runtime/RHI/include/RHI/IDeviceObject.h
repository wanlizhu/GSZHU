#pragma once

#include "RHI/IDevice.h"

namespace Wanlix
{
    struct DeviceObjectDesc {};

    class IDeviceObject : public std::enable_shared_from_this<IDeviceObject>
    {
    public:
        using Device = IDevice;

        virtual SharedPtr<IDeviceObject> GetSharedPtr() final { return shared_from_this(); };
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle GetHandle() const = 0;
        virtual const DeviceObjectDesc& GetDesc() const = 0;
        virtual IDevice* GetDevice() const = 0;
    };
}