#pragma once

#include <NTGS/Config.h>

namespace NTGS {
    struct DeviceObjectAttribs {
        const char* name = nullptr;

        DeviceObjectAttribs() noexcept = default;
        explicit DeviceObjectAttribs(const char* _name) : name(_name) {}
    };

    class IDeviceObject : public IObject {
    public:
        virtual const DeviceObjectAttribs& GetDesc() const = 0; 
    };
}