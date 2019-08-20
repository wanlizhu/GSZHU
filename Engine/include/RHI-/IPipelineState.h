#pragma once

#include "Graphics/RHI/Constants.h"
#include "Graphics/RHI/IShader.h"
#include "Graphics/RHI/InputLayoutDesc.h"
#include "Graphics/RHI/BlendStateDesc.h"
#include "Graphics/RHI/RasterizerStateDesc.h"
#include "Graphics/RHI/DepthStencilStateDesc.h"

namespace ZHU
{
	struct ZHU_GS_API IPipelineStateDesc : public IDeviceObjectDesc
	{
		uint32_t AllocationGranularity = 1;
		uint64_t CommandQueueMask = 1;

		virtual bool IsComputePipelineState() const = 0;
	};

    struct ZHU_GS_API GraphicsPipelineStateDesc : public IPipelineStateDesc
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
        std::vector<ETextureFormat> RenderTargetFormats;
		ETextureFormat DepthStencilFormat = ETextureFormat::Unknown;

        uint8_t SampleCount = 1;
		uint8_t SampleQuality = 0;

		virtual bool IsComputePipelineState() const override { return false; }
    };


    struct ZHU_GS_API ComputePipelineStateDesc : public IPipelineStateDesc
    {
        IShader::Pointer ComputeShader = nullptr;

		virtual bool IsComputePipelineState() const override { return true; }
    };


    class ZHU_GS_API IPipelineState : public IDeviceObject
    {
	public:
		using Pointer = std::shared_ptr<IPipelineState>;
		using Desc = IPipelineStateDesc;

        virtual const IPipelineStateDesc& GetDesc() const = 0;

    };
}