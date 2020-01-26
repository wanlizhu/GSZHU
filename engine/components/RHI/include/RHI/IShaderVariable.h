#pragma once

#include "RHI/IResource.h"

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
        using Desc = ShaderVariableDesc;

        virtual void Set(IResource* res) = 0;
        virtual void SetArray(const Array<IResource*>& array) = 0;
        virtual Uint GetIndex() const = 0;
    };
}