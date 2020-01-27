#pragma once

#include "RHI/ISwapChain.h"
#include "RHI/Class/CDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CSwapChainBase : public CDeviceObjectBase<_Interface_>
    {
    protected:
        CSwapChainBase(IDevice* device,
                       const SwapChainDesc& desc,
                       const String& name)
            : CDeviceObjectBase<_Interface_>(device, desc, name)
        {}
    };
}