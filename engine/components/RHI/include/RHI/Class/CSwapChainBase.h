#pragma once

#include "RHI/ISwapChain.h"
#include "RHI/Class/CDeviceObjectBase.h"

namespace Wanlix
{
    class CSwapChainBase : public CDeviceObjectBase<ISwapChain>
    {
    protected:
        CSwapChainBase(IDevice* device,
                       const SwapChainDesc& desc,
                       const String& name)
            : CDeviceObjectBase<ISwapChain>(device, desc, name)
        {}
    };
}