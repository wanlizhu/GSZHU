#pragma once

#include "Common.h"

namespace Wanlix
{
    class RHI_API IResourceBinding : public Object
    {
        CLASSINFO(IResourceBinding)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        virtual ~IResourceBinding() = default;

        virtual IPipelineState* GetPipelineState() = 0;
        virtual void BindResources(Uint shaderFlags, IResourceMapping* resourceMapping, Uint flags) = 0;
        virtual IShaderVariable* GetVariableByName(ShaderType shaderType, String const& name) = 0;
        virtual IShaderVariable* GetVariableByIndex(ShaderType shaderType, Uint index) = 0;
        virtual Uint GetNumVariables(ShaderType shaderType) const = 0;
        virtual void InitResources(const IPipelineState* pso = nullptr) = 0;
    };
}