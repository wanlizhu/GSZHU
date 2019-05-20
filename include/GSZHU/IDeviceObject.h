#pragma once

#include "IObject.h"
#include "SDeviceObjectAttribs.h"

namespace GSZHU {
    class GSZHU_API IDeviceObject : public IObject {
    public:
        virtual const SDeviceObjectAttribs& GetDesc() const = 0;
    };
}