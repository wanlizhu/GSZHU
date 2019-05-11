#pragma once

#include <GSZHU/IShaderVariable.h>
#include <GSZHU/DESC/SShaderDesc.h>
#include <GSZHU/IResourceTable.h>

namespace GSZHU {
    class IShader : public IDeviceObject {
    public:
        virtual const SShaderDesc& GetDesc()const = 0;
        virtual void BindResources(IResourceTable* ResourceTable, UINT Flags) = 0;
        virtual IShaderVariable* GetShaderVariable(const char* Name) = 0;
        virtual UINT GetVariableCount() const = 0;
        virtual IShaderVariable* GetShaderVariable(UINT Index) = 0;
    };
}