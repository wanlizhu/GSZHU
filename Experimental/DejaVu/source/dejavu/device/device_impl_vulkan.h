#pragma once

#include "device/device_interface.h"
#include "utils/inherit_shared_from_this.h"

namespace djv
{
    class DJV_API Device_Vulkan 
        : public DeviceInterface
        , public inherit_shared_from_this<DeviceInterface, Device_Vulkan>
    {
    public:
        using inherit_shared_from_this<DeviceInterface, Device_Vulkan>::shared_from_this;
    
    };
}