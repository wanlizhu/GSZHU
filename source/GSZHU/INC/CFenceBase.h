#pragma once

#include <GSZHU/BasicTypes.h>
#include "CDeviceObjectBase.h"

namespace GSZHU {
    template<typename INTERFACE, typename RENDER_DEVICE>
    class CFenceBase : public CDeviceObjectBase<INTERFACE, RENDER_DEVICE> {
    public:
        using BASE = CDeviceObjectBase<INTERFACE, RENDER_DEVICE>;
        using DESC = typename INTERFACE::DESC;

        CFenceBase(RENDER_DEVICE* Device, const DESC& Desc)
            : BASE(Device, Desc)
        {}
    };
}