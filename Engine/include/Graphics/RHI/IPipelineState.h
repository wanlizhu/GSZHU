#pragma once

#include "Graphics/Constants.h"
#include "Graphics/IShader.h"
#include "Graphics/BlendStateDesc.h"
#include "Graphics/RasterizerStateDesc.h"
#include "Graphics/DepthStencilStateDesc.h"
#include "Graphics/InputLayoutDesc.h"
#include "Graphics/SampleDesc.h"

namespace ZHU
{
    struct GraphicsPipelineDesc
    {
        IShader::Pointer VertexShader = nullptr;
        IShader::Pointer PixelShader = nullptr;
        IShader::Pointer GeometryShader = nullptr;
        IShader::Pointer HullShader = nullptr;
        IShader::Pointer DomainShader = nullptr;

        BlendStateDesc BlendState;
        RasterizerStateDesc RasterizerState;
        DepthStencilStateDesc DepthStencilState;

        uint32_t SamplerMask = 0xffffffff;
        InputLayoutDesc InputLayout;
        EPrimitiveTopology PrimitiveTopology = EPrimitiveTopology::Undefined;
        uint8_t NumViewports = 1;
        uint8_t NumRenderTargets = 1;
        std::vector<EPixelFormat> RTVFormats;
        EPixelFormat DSVFormat = EPixelFormat::Unknown;
        SampleDesc Sample;
    };


    struct ComputePipelineDesc
    {
        IShader::Pointer ComputeShader = nullptr;
    };


    struct PipelineStateDesc : public DeviceObjectDesc
    {
        bool IsComputePipeline = false;
        uint32_t AllocationGranularity = 1;
        uint64_t CommandQueueMask = 1;

        GraphicsPipelineDesc GraphicsPipeline;
        ComputePipelineDesc ComputePipeline;
    };


    class ZHU_API IPipelineState : public IDeviceObject
    {
    public:
        virtual const PipelineStateDesc& GetDesc() const = 0;

    };
}