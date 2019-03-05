#pragma once

#include <NTGS/Graphics/IShader.h>
#include <NTGS/Graphics/IPipelineState.h>

namespace NTGS {
    class IShaderResourceBinding : public IObject
    {
    public:
        virtual IPipelineState* GetPipelineState() = 0;
        virtual void BindResources(UINT ShaderFlags, IResourceMapping* pResMapping, UINT Flags) = 0;
        virtual IShaderVariable* GetVariable(SHADER_TYPE ShaderType, const char *Name) = 0;
        virtual UINT GetVariableCount(SHADER_TYPE ShaderType) const = 0;
        virtual IShaderVariable* GetVariable(SHADER_TYPE ShaderType, UINT Index) = 0;
        virtual void InitializeStaticResources(const IPipelineState* pPipelineState = nullptr) = 0;
    };
}