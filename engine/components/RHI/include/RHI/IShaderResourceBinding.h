#pragma once

#include "RHI/IResourceTable.h"
#include "RHI/IShaderVariable.h"

namespace Wanlix
{
    class IShaderResourceBinding
    {
    public:
        virtual void InitializeStaticResources(const IPipelineState* pso = nullptr) = 0;
        virtual void BindResources(IResourceTable* table, EBindResourceFlags flags, EShaderStages stages = EShaderStages::All) = 0;
        virtual Uint GetVariableCount(EShaderStages stages = EShaderStages::All) const = 0;
        virtual IShaderVariable* GetVariable(const String& name, EShaderStages stages = EShaderStages::All) = 0;
        virtual IShaderVariable* GetVariable(Uint index, EShaderStages stages = EShaderStages::All) = 0;
    };
}