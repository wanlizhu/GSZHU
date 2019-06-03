#pragma once

#include "IObject.h"
#include "GraphicTypes.h"
#include "IResourceDesc.h"

namespace GSZHU {
    class IDeviceObject : public IObject {
    public:
        virtual const IResourceDesc& GetDesc() const = 0;
    };
}