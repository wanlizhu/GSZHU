#pragma once

#include "Graphics/RHI/IResourceTable.h"
#include "Graphics/RHI/IShaderVariable.h"

namespace Wanlix
{
    class IShaderResourceBinding
    {
    public:
        virtual void BindResources(IResourceTable* table, EShaderStages stages = EShaderStages::All) = 0;
        virtual Uint GetVariableCount(EShaderStages stages = EShaderStages::All) const = 0;
        virtual IShaderVariable* GetVariable(const String& name, EShaderStages stages = EShaderStages::All) = 0;
        virtual IShaderVariable* GetVariable(Uint index, EShaderStages stages = EShaderStages::All) = 0;
    };
}