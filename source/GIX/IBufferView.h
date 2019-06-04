#pragma once

#include "IDeviceObject.h"
#include "CBufferViewDesc.h"

namespace GSZHU {
    class IBuffer;

    class IBufferView : public IDeviceObject {
    public:
        virtual CBufferViewDesc const& GetDesc() const = 0;
        virtual std::shared_ptr<IBuffer> GetBuffer() = 0;
    };
}