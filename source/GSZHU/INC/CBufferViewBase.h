#pragma once

#include <GSZHU/IBufferView.h>
#include "CDeviceObjectBase.h"
#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    class IRenderDevice;

    template<typename INTERFACE, typename RENDER_DEVICE>
    class CBufferViewBase : public CDeviceObjectBase<INTERFACE, RENDER_DEVICE> {
    public:
        using BASE = CDeviceObjectBase<INTERFACE, RENDER_DEVICE>;
        using DESC = typename INTERFACE::DESC;

        CBufferViewBase(RENDER_DEVICE* Device, IBuffer* Buffer, const DESC& ViewDesc)
            : BASE(Device, ViewDesc)
            , mBuffer(Buffer)
        {}

        virtual IBuffer* GetBuffer() override final { return mBuffer; }
        template<typename T> T* GetBuffer() { return dynamic_cast<T*>(mBuffer); }
        template<typename T> const T* GetBuffer() const { return dynamic_cast<const T*>(mBuffer); }

    protected:
        IBuffer*const mBuffer = nullptr;
    };
}