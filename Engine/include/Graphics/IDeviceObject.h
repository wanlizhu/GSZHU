#pragma once

#include "Common/Config.h"
#include "Graphics/Object.h"
#include "Graphics/Constants.h"
#include "Graphics/Tools.h"
#include "Graphics/DeviceObjectDescriptor.h"

namespace ZHU
{
    class ZHU_API IDeviceObject : public Object
    {
    public:
        using Pointer = std::shared_ptr<IDeviceObject>;

        virtual const DeviceObjectDescriptor& GetDesc() const = 0;

    protected:
        virtual bool Initialize() = 0;
    };
}