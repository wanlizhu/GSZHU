#pragma once

#include <NTGS/Graphics/IBufferView.h>
#include <NTGS/Graphics/GraphicsTypes.h>
#include "DeviceObjectBase.h"

namespace NTGS {
    class IRenderDevice;

    template<typename BaseInterface, typename RenderDeviceType>
    class BufferViewBase : public DeviceObjectBase<BaseInterface, RenderDeviceType, BufferViewDesc> {
    public:
        using BaseType = DeviceObjectBase<BaseInterface, RenderDeviceType, BufferViewDesc>;

        BufferViewBase(Pointer<RenderDeviceType> Device, const BufferViewDesc& ViewDesc, Pointer<IBuffer> Buffer)
    };

}