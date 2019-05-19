#pragma once

#include <GSZHU/IDeviceObject.h>
#include <GSZHU/DESC/SFenceDesc.h>

namespace GSZHU {
    class IFence : public IDeviceObject {
    public:
        using DESC = SFenceDesc;

        virtual const SFenceDesc& GetDesc() const = 0;
        virtual uint64_t GetCompletedValue() = 0;
        virtual void Reset(uint64_t Value) = 0;
    };
}