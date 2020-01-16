#pragma once

#include "Graphics/RHI/IDevice.h"

namespace Wanlix
{
    struct DeviceObjectDesc
    {};

    class IDeviceObject
    {
    public:
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const DeviceObjectDesc& GetDesc() const = 0;
    };
}