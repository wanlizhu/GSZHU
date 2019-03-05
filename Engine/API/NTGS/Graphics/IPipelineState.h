#pragma once

#include <NTGS/Graphics/BlendState.h>
#include <NTGS/Graphics/GraphicsEnums.h>
#include <NTGS/Graphics/DepthStencilState.h>
#include <NTGS/Graphics/RasterizerState.h>
#include <NTGS/Graphics/InputLayout.h>
#include <NTGS/Graphics/IShaderResourceBinding.h>

namespace NTGS {
    struct SampleDesc {
        UINT8 Count = 1;
        UINT8 Quality = 0;

        SampleDesc() noexcept {}
        SampleDesc(UINT8 _Count, UINT8 _Quality)
            : Count(_Count), Quality(_Quality)
        {}
    };

    struct GraphicsPipelineDesc {
        IShader* VS = nullptr;
        IShader* PS = nullptr;
        IShader* DS = nullptr;
        IShader* HS = nullptr;
        IShader* GS = nullptr;

        BlendStateDesc BlendDesc;
        UINT SampleMask = 0xFFFFFFFF;
        RasterizerStateDesc RasterizerDesc;
        DepthStencilStateDesc DepthStencilDesc;

        InputLayoutDesc InputLayout;
        PRIMITIVE_TOPOLOGY PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        UINT8 NumViewports = 1;
        UINT8 NumRenderTargets = 0;
        TEXTURE_FORMAT RTVFormats[8] = {};
        TEXTURE_FORMAT DSVFormat = TEX_FORMAT_UNKNOWN;
        SampleDesc SmplDesc;
        UINT NodeMask = 0;
    };

    struct ComputePipelineDesc {
        IShader* CS = nullptr;
    };

    struct PipelineStateDesc : public IDeviceObjectAttribs {
        bool IsComputePipeline = false;
        UINT SRBAllocationGranularity = 1;
        UINT64 CommandQueueMask = 1;
        GraphicsPipelineDesc GraphicsPipeline;
        ComputePipelineDesc ComputePipeline;
    };

    class IPipelineState : public IDeviceObject {
    public:
        virtual const PipelineStateDesc& GetDesc()const = 0;
        virtual void BindShaderResources(IResourceMapping* pResourceMapping, UINT Flags) = 0;
        virtual void CreateShaderResourceBinding(IShaderResourceBinding** ppShaderResourceBinding, bool InitStaticResources = false) = 0;
        virtual bool IsCompatibleWith(const IPipelineState* pPSO)const = 0;
    };
}