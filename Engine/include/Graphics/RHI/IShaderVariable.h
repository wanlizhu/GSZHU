#pragma once

#include "Graphics/IDeviceObject.h"

namespace ZHU
{
    class ZHU_API IShaderVariable : public Object
    {
    public:
        virtual const std::string& GetName() const = 0;
        virtual size_t GetIndex() const = 0;

        virtual void BindResource(IDeviceObject::Pointer object) = 0;
        virtual IDeviceObject::Pointer GetBoundResource() const = 0;
    };
}