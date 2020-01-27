#pragma once

#include "RHI/IFence.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CFenceBase : public CDeviceObjectBase<_Interface_>
    {
        static_assert(std::is_base_of_v<IFence, _Interface_> || std::is_same_v<IFence, _Interface_>);
    protected:
        CFenceBase(IDevice* device, const String& name)
            : CDeviceObjectBase<_Interface_>(device, FenceDesc(), name)
        {}
    };
}