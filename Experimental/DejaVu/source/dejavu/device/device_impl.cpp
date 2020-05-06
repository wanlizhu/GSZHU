#include "device/device_interface.h"

namespace djv
{
    std::weak_ptr<DeviceInterface> gpDevice;

    std::shared_ptr<DeviceInterface> createDevice()
    {
        return nullptr;
    }
}