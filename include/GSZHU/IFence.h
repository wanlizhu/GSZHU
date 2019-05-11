#pragma once

#include <GSZHU/IDeviceObject.h>
#include <GSZHU/DESC/SFenceDesc.h>

namespace GSZHU {
    class IFence : public IDeviceObject {
    public:
        virtual const SFenceDesc& GetDesc() const = 0;
        virtual UINT64 GetCompletedValue() = 0;
        virtual void Reset(UINT64 Value) = 0;
    };
}