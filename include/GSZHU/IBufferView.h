#pragma once

#include <GSZHU/DESC/SBufferViewDesc.h>
#include <GSZHU/IDeviceObject.h>
#include <GSZHU/IBuffer.h>

namespace GSZHU {
    class IBufferView : public IDeviceObject {
    public:
        using DESC = SBufferViewDesc;

        virtual const SBufferViewDesc& GetDesc() const = 0;
        virtual IBuffer* GetBuffer() = 0;
    };
}