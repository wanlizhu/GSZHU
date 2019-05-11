#pragma once

#include <GSZHU/DESC/SSamplerDesc.h>
#include <GSZHU/IDeviceObject.h>

namespace GSZHU {
    class ISampler : public IDeviceObject {
    public:
        virtual const SSamplerDesc& GetDesc() const = 0;
    };
}