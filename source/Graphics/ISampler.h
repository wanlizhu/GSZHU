#pragma once

#include "IDeviceObject.h"
#include "CSamplerDesc.h"

namespace GSZHU {
    class ISampler : public IDeviceObject {
    public:
        virtual CSamplerDesc const& GetDesc() const = 0;
    };
}