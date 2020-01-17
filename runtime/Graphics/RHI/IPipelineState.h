#pragma once

#include "Core/Common.h"
#include "Graphics/RHI/IDeviceObject.h"
#include "Graphics/RHI/RHIFlags.h"
#include "Graphics/RHI/BlendState.h"
#include "Graphics/RHI/RasterizerState.h"
#include "Graphics/RHI/DepthStencilState.h"
#include "Graphics/RHI/IShaderResourceBinding.h"

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

    struct SampleDesc
    {
        Uint8 count = 1;
        Uint8 quality = 0;
    };

    struct PipelineStateDesc : public DeviceObjectDesc
    {
        IShader* VertexShader = nullptr;
        IShader* TessCtrlShader = nullptr;
        IShader* TessEvalShader = nullptr;
        IShader* GeometryShader = nullptr;
        IShader* FragmentShader = nullptr;
        IShader* ComputeShader = nullptr;

        BlendStateDesc blendStateDesc;
        RasterizerStateDesc rasterizerStateDesc;
        DepthStencilStateDesc depthStencilStateDesc;
        SampleDesc sampleDesc;
        InputLayoutDesc inputLayoutDesc;

        EPrimitiveTopology primitiveTopology = EPrimitiveTopology::TriangleList;
        Uint8 numViewports = 1;
        Array<EPixelFormat> RTVFormats;
        EPixelFormat DSVFormat = EPixelFormat::D32Float;

        Uint sampleMask = 0xfffffff;
        Uint64 queueMask = 1;
        Uint nodeMask = 0;
    };

    class IPipelineState : public IDeviceObject
    {
    public:
        virtual void BindResources(IResourceTable* table, EBindResourceFlags flags) = 0;
        virtual Bool IsCompatibleWith(const IPipelineState* pso) const = 0;
        virtual SharedPtr<IShaderResourceBinding> GetShaderResourceBinding() = 0;

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const PipelineStateDesc& GetDesc() const = 0;
    };
}