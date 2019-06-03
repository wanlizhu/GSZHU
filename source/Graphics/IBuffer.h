#pragma once

#include "IDeviceObject.h"
#include "CBufferDesc.h"
#include "IBufferView.h"

namespace GSZHU {
    class IBuffer : public IDeviceObject {
    public:
        virtual CBufferDesc const& GetDesc() const = 0;
        virtual std::shared_ptr<IBufferView> CreateView(CBufferViewDesc const& desc) = 0; // Buffer view will contain strong reference to the buffer
        virtual void* GetNativeHandle() const = 0;
    };
}