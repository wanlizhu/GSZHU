#pragma once

#include <GSZHU/IShader.h>
#include <GSZHU/IPipelineState.h>
#include <GSZHU/IResourceTable.h>
#include <GSZHU/ENUM/EShaderType.h>

namespace GSZHU {
    class GSZHU_API IShaderResourceBinding : public IObject
    {
    public:
        virtual IPipelineState* GetPipelineState() = 0;
        virtual void BindResources(uint32_t ShaderFlags, IResourceTable* pResMapping, uint32_t Flags) = 0;
        virtual IShaderVariable* GetVariable(ESHADER_TYPE ShaderType, const char *Name) = 0;
        virtual uint32_t GetVariableCount(ESHADER_TYPE ShaderType) const = 0;
        virtual IShaderVariable* GetVariable(ESHADER_TYPE ShaderType, uint32_t Index) = 0;
        virtual void InitializeStaticResources(const IPipelineState* pPipelineState = nullptr) = 0;
    };
}