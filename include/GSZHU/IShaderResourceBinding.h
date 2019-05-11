#pragma once

#include <GSZHU/IShader.h>
#include <GSZHU/IPipelineState.h>
#include <GSZHU/IResourceTable.h>
#include <GSZHU/ENUM/EShaderType.h>

namespace GSZHU {
    class IShaderResourceBinding : public IObject
    {
    public:
        virtual IPipelineState* GetPipelineState() = 0;
        virtual void BindResources(UINT ShaderFlags, IResourceTable* pResMapping, UINT Flags) = 0;
        virtual IShaderVariable* GetVariable(ESHADER_TYPE ShaderType, const char *Name) = 0;
        virtual UINT GetVariableCount(ESHADER_TYPE ShaderType) const = 0;
        virtual IShaderVariable* GetVariable(ESHADER_TYPE ShaderType, UINT Index) = 0;
        virtual void InitializeStaticResources(const IPipelineState* pPipelineState = nullptr) = 0;
    };
}