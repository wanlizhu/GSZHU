#pragma once

#include "RHI/IDevice.h"

namespace Wanlix
{
    class IDeviceObject
    {
    public:
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
    };
}