#pragma once

#include "RHI/ICommandQueue.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CCommandQueueBase : public CDeviceObjectBase<_Interface_>
    {
        static_assert(std::is_base_of_v<ICommandQueue, _Interface_> || std::is_same_v<ICommandQueue, _Interface_>);
    protected:
        CCommandQueueBase(IDevice* device,
                          const String& name,
                          Uint familyIndex)
            : CDeviceObjectBase<_Interface_>(device, CommandQueueDesc(), name)
            , mQueueFamilyIndex(familyIndex)
        {}

    protected:
        Uint mQueueFamilyIndex = 0;
    };
}