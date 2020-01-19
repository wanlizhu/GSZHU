#pragma once

#include "RHI/ICommandQueue.h"
#include "TDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CommandQueueBase : public TDeviceObjectBase< _Interface_>
    {
        using BaseClass = TDeviceObjectBase<_Interface_>;
        using DeviceType = typename _Interface_::Device;
        using DescType = typename _Interface_::Desc;

    public:

    protected:
        CommandQueueBase(DeviceType* device,
                         const String& name,
                         Uint familyIndex)
            : BaseClass(device, DescType(), name)
            , mQueueFamilyIndex(familyIndex)
        {}

    protected:
        Uint mQueueFamilyIndex = 0;
    };
}