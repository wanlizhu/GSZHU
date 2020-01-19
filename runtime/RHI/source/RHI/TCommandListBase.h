#pragma once

#include "RHI/ICommandList.h"
#include "TDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CommandListBase : public TDeviceObjectBase<_Interface_>
    {
        using BaseClass = TDeviceObjectBase<_Interface_>;
        using DeviceType = typename _Interface_::Device;
        using DescType = typename _Interface_::Desc;

    public:

    protected:
        CommandListBase(DeviceType* device, const String& name)
            : BaseClass(device, DescType(), name)
        {}
    };
}