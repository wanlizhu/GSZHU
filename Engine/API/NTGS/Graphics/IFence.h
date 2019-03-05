#pragma once

#include <NTGS/Graphics/IDeviceObject.h>

namespace NTGS {
    struct FenceDesc : IDeviceObjectAttribs {

    };

    class IFence : public IDeviceObject {
    public:
        virtual const FenceDesc& GetDesc() const = 0;
        virtual UINT64 GetCompletedValue() = 0;
        virtual void Reset(UINT64 Value) = 0;
    };
}