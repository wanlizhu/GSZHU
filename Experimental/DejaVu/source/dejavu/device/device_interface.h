#pragma once

#include "device/device_types.h"
#include "utils/preprocess.h"

namespace djv
{
    class DJV_API DeviceInterface : public std::enable_shared_from_this<DeviceInterface>
    {
    public:

    };

    DJV_API extern std::weak_ptr<DeviceInterface> gpDevice;

    std::shared_ptr<DeviceInterface> DJV_API createDevice();
}