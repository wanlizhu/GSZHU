#pragma once

#include "device/device_interface.h"
#include "utils/inherit_shared_from_this.h"

namespace djv
{
    class DJV_API Device_D3D12
        : public DeviceInterface
        , public inherit_shared_from_this<DeviceInterface, Device_D3D12>
    {
    public:
        using inherit_shared_from_this<DeviceInterface, Device_D3D12>::shared_from_this;
    
    };
}