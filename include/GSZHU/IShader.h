#pragma once

#include <GSZHU/IShaderVariable.h>
#include <GSZHU/DESC/SShaderDesc.h>
#include <GSZHU/IResourceTable.h>

namespace GSZHU {
    class GSZHU_API IShader : public IDeviceObject {
    public:
        using DESC = SShaderDesc;

        virtual const SShaderDesc& GetDesc()const = 0;
        virtual void BindResources(IResourceTable* ResourceTable, uint32_t Flags) = 0;
        virtual IShaderVariable* GetShaderVariable(const char* Name) = 0;
        virtual uint32_t GetVariableCount() const = 0;
        virtual IShaderVariable* GetShaderVariable(uint32_t Index) = 0;
    };
}