#pragma once

#include <NTGS/Graphics/IDeviceObjectAttribs.h>
#include <NTGS/Graphics/IObject.h>

namespace NTGS {
    class IDeviceObject : public IObject {
    public:
        virtual const IDeviceObjectAttribs& GetDesc() const = 0;
    };
}