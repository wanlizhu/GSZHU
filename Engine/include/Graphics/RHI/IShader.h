#pragma once

#include "Graphics/RHI/IDeviceObject.h"

namespace ZHU
{
    struct ZHU_GS_API ShaderDesc : public DeviceObjectDesc
    {

    };


    class ZHU_GS_API IShader : public IDeviceObject
    {
    public:
        virtual const ShaderDesc& GetDesc() const = 0;
        
        virtual Object::Pointer GetVariable(const std::string& name) const = 0;
        virtual size_t GetVariableCount() const = 0;
        virtual Object::Pointer GetVariable(size_t index) const = 0;

        virtual size_t GetBoundResourceCount() const = 0;
        virtual IDeviceObject::Pointer GetBoundResource(size_t index) const = 0;
    };
}