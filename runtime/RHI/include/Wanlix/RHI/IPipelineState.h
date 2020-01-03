#pragma once

#include "Common.h"
#include "IDeviceObject.h"
#include "BlendState.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "InputLayout.h"
#include "IResourceBinding.h"
#include "IShaderVariable.h"
#include "IShader.h"
#include "ISampler.h"

namespace Wanlix
{
    struct SampleDesc
    {
        Byte count = 1;
        Byte quality = 0;
    };

    struct PipelineResourceLayoutDesc
    {
        ShaderVariableType defaultVariableType;
        Array<ShaderVariableDesc> variables;
        Array<StaticSamplerDesc> staticSamplers;
    };

    struct GraphicsPipelineDesc
    {
        IShader* vertexShader = nullptr;
        IShader* fragmentShader = nullptr;
        IShader* tessEvalShader = nullptr;
        IShader* tessCtrlShader = nullptr;
        IShader* geometryShader = nullptr;

        BlendStateDesc blendDesc;
        Uint sampleMask = 0xffffffff;
        RasterizerStateDesc rasterizerDesc;
        DepthStencilStateDesc depthStencilDesc;
        InputLayoutDesc inputLayout;

        PrimitiveTopology primitiveTopology = PrimitiveTopology::Undefined;
        Byte numPatches = 0;
        Byte numViewports = 1;
        Byte numRenderTargets = 0;
        PixelFormat renderTargetFormats[RHI_MAX_NUM_COLOR_ATTACHMENTS] = { };
        PixelFormat depthStencilFormat = PixelFormat::Undefined;

        SampleDesc sampleDesc;
        Uint nodeMask = 0;
    };

    struct ComputePipelineDesc
    {
        IShader* computeShader = nullptr;
    };

    struct PipelineStateDesc : public DeviceObjectDesc
    {
        Bool isGraphicsPipeline = true;
        Uint allocationGranularity = 1;
        Uint64 commandQueueMask = 1;
        PipelineResourceLayoutDesc resourceLayout;
        GraphicsPipelineDesc graphicsPipeline;
        ComputePipelineDesc computePipeline;
    };

    class RHI_API IPipelineState : public IDeviceObject
    {
        CLASSINFO(IPipelineState)
        INHERIT_SHARED_FROM_THIS(IDeviceObject)
    public:
        /* interfaces inherited from IDeviceObject */
        virtual Pvoid GetNativeHandle() const = 0;
        virtual PipelineStateDesc const& GetDesc() const override = 0;
        virtual IDevice* GetDevice() const override = 0;

        virtual void BindResources(Uint shaderFlags, IResourceMapping* resourceMapping, Uint flags) = 0;
        virtual Uint GetNumVariables(ShaderType shaderType) const = 0;
        virtual IShaderVariable* GetVariableByName(ShaderType shaderType, const char* name) = 0;
        virtual IShaderVariable* GetVariableByIndex(ShaderType shaderType, Uint index) = 0;
        virtual IResourceBinding* CreateResourceBinding(Bool initResources = false) = 0;
        virtual Bool IsCompatibleWith(const IPipelineState* pipeline) const = 0;
    };
}