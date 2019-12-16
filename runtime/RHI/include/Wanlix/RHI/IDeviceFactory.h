#pragma once

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

    class IDeviceFactory : public inherit_shared_from_this<Object, IDeviceFactory>
    {
    public:
        virtual IDevice* CreateDevice() = 0;
    };
}