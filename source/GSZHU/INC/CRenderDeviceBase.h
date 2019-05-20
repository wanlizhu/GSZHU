#pragma once

#include <GSZHU/IRenderDevice.h>
#include <GSZHU/IDeviceObject.h>
#include <GSZHU/IResourceTable.h>
#include <GSZHU/IObject.h>
#include <GSZHU/IDeviceContext.h>
#include <GSZHU/ISwapChain.h>
#include <GSZHU/BasicTools.h>
#include "CObjectBase.h"

namespace GSZHU {
    template<typename INTERFACE>
    class CRenderDeviceBase : public CObjectBase<INTERFACE> {
    public:
        using BASE = CObjectBase<INTERFACE>;

    };
}