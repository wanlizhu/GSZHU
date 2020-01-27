#pragma once

#include "RHI/ICommandList.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CCommandListBase : public CDeviceObjectBase<_Interface_>
    {
        static_assert(std::is_base_of_v<ICommandList, _Interface_> || std::is_same_v<ICommandList, _Interface_>);
    protected:
        CCommandListBase(IDevice* device, const String& name)
            : CDeviceObjectBase<_Interface_>(device, CommandListDesc(), name)
        {}
    };
}