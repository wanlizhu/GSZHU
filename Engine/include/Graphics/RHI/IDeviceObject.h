#pragma once

#include "Common/Config.h"
#include "Graphics/RHI/Object.h"
#include "Graphics/RHI/Constants.h"
#include "Graphics/Tools.h"
#include "Graphics/RHI/DeviceObjectDesc.h"

namespace ZHU
{
    class ZHU_GS_API IDeviceObject : public Object
    {
    public:
        using Pointer = std::shared_ptr<IDeviceObject>;
        using Desc = DeviceObjectDesc;

		IDeviceObject(const std::string& name) : Object(name) {}
        virtual const DeviceObjectDesc& GetDesc() const = 0;

    protected:
        virtual bool Initialize() = 0;
    };
}