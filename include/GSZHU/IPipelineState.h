#pragma once

#include <GSZHU/IDeviceObject.h>
#include <GSZHU/DESC/SPipelineStateDesc.h>
#include <GSZHU/IShaderResourceBinding.h>

namespace GSZHU {
    class IPipelineState : public IDeviceObject {
    public:
        virtual const SPipelineStateDesc& GetDesc()const = 0;
        virtual void BindShaderResources(IResourceTable* ResourceTable, UINT Flags) = 0;
        virtual IShaderResourceBinding* CreateShaderResourceBinding(bool InitStaticResources = false) = 0;
        virtual bool IsCompatibleWith(const IPipelineState* pPSO)const = 0;
    };

}