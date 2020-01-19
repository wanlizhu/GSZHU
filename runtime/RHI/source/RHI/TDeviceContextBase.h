#pragma once

#include "RHI/IDeviceContext.h"
#include "RHI/TDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class TDeviceContextBase : public _Interface_
    {
        using DeviceType = typename _Interface_::Device;
    public:

    protected:
        SharedPtr<DeviceType> mDevice;
    };
}