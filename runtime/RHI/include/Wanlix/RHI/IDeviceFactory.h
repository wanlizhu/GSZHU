#pragma once

#include "IAllocator.h"
#include "IDevice.h"
#include "DebugOutput.h"

namespace Wanlix
{
    struct DeviceCreateInfo
    {
        IAllocator* allocator = nullptr;
        DebugMessageCallback debugCallback = nullptr;
        Uint numDeferredContexts = 0;
    };

    class RHI_API IDeviceFactory : public Object
    {
        CLASSINFO(IDeviceFactory)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        virtual IDevice* CreateDevice() = 0;
    };
}