#pragma once

#include "Graphics/RHI/IResource.h"

namespace Wanlix
{
    struct ShaderVariableDesc : public DeviceObjectDesc
    {
        String name;
        EShaderVariableType variableType = EShaderVariableType::Static;
        Optional<Uint> arraySize = std::nullopt;
    };

    class IShaderVariable : public IDeviceObject
    {
    public:
        virtual void Set(IResource* res) = 0;
        virtual void SetArray(const Array<IResource*>& array) = 0;
        virtual Uint GetIndex() const = 0;

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const ShaderVariableDesc& GetDesc() const = 0;
    };
}