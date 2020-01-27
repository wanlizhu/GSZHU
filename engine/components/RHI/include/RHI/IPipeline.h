#pragma once

#include "Wanlix/Common.h"
#include "RHI/IDeviceObject.h"
#include "RHI/Flags.h"
#include "RHI/BlendState.h"
#include "RHI/RasterizerState.h"
#include "RHI/DepthStencilState.h"
#include "RHI/IShaderResourceBinding.h"
#include "RHI/IShader.h"

namespace Wanlix
{
    struct LayoutElement
    {
        Uint index = 0;
        Uint slot = 0;
        Uint components = 0;
        EDataType dataType = EDataType::Float32;
        Bool isNormalized = false;
        Uint offset = 0;
        Uint stride = 0;
        EInputFrequency frequency = EInputFrequency::PerVertex;
        Uint instanceStep = 1;
    };

    struct InputLayoutDesc
    {
        Array<LayoutElement> elements;
    };

    struct ShaderResourceLayoutDesc
    {
        EShaderResourceType shaderResourceType = EShaderResourceType::Unknown;
        Array<ShaderVariableDesc> variables;
        Array<StaticSampler> staticSamplers;
    };

    struct SampleDesc
    {
        Uint8 count = 1;
        Uint8 quality = 0;
    };

    struct GraphicsPipelineDesc
    {
        IShader* vertexShader = nullptr;
        IShader* tessCtrlShader = nullptr;
        IShader* tessEvalShader = nullptr;
        IShader* geometryShader = nullptr;
        IShader* fragmentShader = nullptr;

        BlendStateDesc blendState;
        RasterizerStateDesc rasterizerState;
        DepthStencilStateDesc depthStencilState;
        SampleDesc sample;
        InputLayoutDesc inputLayout;

        EPrimitiveTopology primitiveTopology = EPrimitiveTopology::TriangleList;
        Uint8 numViewports = 1;
        Array<EPixelFormat> RTVFormats;
        EPixelFormat DSVFormat = EPixelFormat::D32Float;

        Uint sampleMask = 0xfffffff;
        Uint nodeMask = 0;
    };

    struct ComputePipelineDesc
    {
        IShader* computeShader = nullptr;
    };

    struct PipelineDesc : public DeviceObjectDesc
    {
        Bool isGraphicsPipeline = true;
        Uint64 queueMask = 1;
        ShaderResourceLayoutDesc resourceLayout;
        GraphicsPipelineDesc graphicsPipeline;
        ComputePipelineDesc computePipeline;
    };

    class IPipeline : public IDeviceObject
    {
    public:
        virtual void BindResources(IResourceTable* table, EBindResourceFlags flags) = 0;
        virtual Bool IsCompatibleWith(const IPipeline* pso) const = 0;
        virtual SharedPtr<IShaderResourceBinding> GetShaderResourceBinding() = 0;
    };
}