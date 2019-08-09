#pragma once

#include "Graphics/IDeviceObject.h"

namespace ZHU
{
    struct ShaderDescriptor : public DeviceObjectDescriptor
    {

    };


    class ZHU_API IShader : public IDeviceObject
    {
    public:
        virtual const ShaderDescriptor& GetDesc() const = 0;
        
        virtual Object::Pointer GetVariable(const std::string& name) const = 0;
        virtual size_t GetVariableCount() const = 0;
        virtual Object::Pointer GetVariable(size_t index) const = 0;

        virtual size_t GetBoundResourceCount() const = 0;
        virtual IDeviceObject::Pointer GetBoundResource(size_t index) const = 0;
    };
}