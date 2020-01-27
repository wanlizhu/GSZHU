#pragma once

#include "RHI/IDeviceObject.h"

namespace Wanlix
{
    struct CommandListDesc : public DeviceObjectDesc 
    {};

    class ICommandList : public IDeviceObject
    {
    public:
        virtual void Close() = 0;
    };
}