#pragma once

#include "Types.h"

namespace Wanlix
{
    struct ImageReadDescriptor
    {
        ColorFormat format   = ColorFormat::RGBA;
        DataType    dataType = DataType::UInt8;
        size_t      dataSize = 0;
        const void* data = nullptr;
    };

    struct ImageWriteDescriptor
    {
        ColorFormat format   = ColorFormat::RGBA;
        DataType    dataType = DataType::UInt8;
        size_t      dataSize = 0;
        void*       data = nullptr;
    };

    struct VsyncDescriptor
    {
        bool enabled = false;
        uint32_t refreshRate = 60;
        uint32_t interval = 1;
    };

    struct VideoModeDescriptor
    {
        Extent resolution;
        int    colorBits = 32;
        int    depthBits = 24;
        int    stencilBits = 8;
        bool   fullscreen = false;
        int    swapChainSize = 2;
    };

    struct SwapChainDescriptor
    {
        VsyncDescriptor     vsync;
        uint32_t       samples = 1;
        VideoModeDescriptor videoMode;
    };

    struct ResourceDescriptor
    {
        ResourceType resourceType;
        long bindFlags = 0;
        long cpuAccessFlags = 0;
        long miscFlags = 0;

        ResourceDescriptor(ResourceType type)
            : resourceType(type)
        {}
    };

    struct BufferDescriptor : public ResourceDescriptor
    {
        uint64_t size = 0;
        uint32_t stride = 0;
        Format   format = Format::Undefined;
        std::vector<VertexAttribute> vertexAttribs;

        BufferDescriptor()
            : ResourceDescriptor(ResourceType::Buffer)
        {
            bindFlags = 0;
            cpuAccessFlags = 0;
            miscFlags = 0;
        }
    };

    struct TextureDescriptor : public ResourceDescriptor
    {
        TextureType textureType = TextureType::Texture2D;
        Format      format = Format::RGBA8UNorm;
        Extent      extent = { 1, 1, 1 };
        uint32_t    arrayLayers = 1;
        uint32_t    mipLevels = 0;
        uint32_t    samples = 1;
        Color4F     clearValue;

        TextureDescriptor() 
            : ResourceDescriptor(ResourceType::Texture)
        {
            bindFlags = BindFlags::Sampled | BindFlags::ColorAttachment;
            cpuAccessFlags = 0;
            miscFlags = MiscFlags::FixedSamples | MiscFlags::GenerateMips;
        }
    };

    struct TextureViewDescriptor
    {
        TextureType type = TextureType::Texture2D;
        Format      format = Format::RGBA8UNorm;
        TextureSubresource subresource;
        TextureSwizzleRGBA swizzle;
    };

    struct SamplerDescriptor
    {
        SamplerAddressMode  addressModeU = SamplerAddressMode::Repeat;
        SamplerAddressMode  addressModeV = SamplerAddressMode::Repeat;
        SamplerAddressMode  addressModeW = SamplerAddressMode::Repeat;
        SamplerFilter       minFilter = SamplerFilter::Linear;
        SamplerFilter       magFilter = SamplerFilter::Linear;
        SamplerFilter       mipMapFilter = SamplerFilter::Linear;

        bool      mipMapping = true;
        float     mipMapLODBias = 0.0f;
        float     minLOD = 0.0f;
        float     maxLOD = 1000.0f;
        uint32_t  maxAnisotropy = 1;
        bool      compareEnabled = false;
        CompareOp compareOp = CompareOp::Less;
        Color4F   borderColor = { 0.0f, 0.0f, 0.0f, 0.0f };
    };

    struct DepthDescriptor
    {
        bool      testEnabled = false;
        bool      writeEnabled = false;
        CompareOp compareOp = CompareOp::Less;
    };

    struct StencilFaceDescriptor
    {
        StencilOp stencilFailOp = StencilOp::Keep;
        StencilOp depthFailOp = StencilOp::Keep;
        StencilOp depthPassOp = StencilOp::Keep;
        CompareOp compareOp = CompareOp::Less;

        uint32_t  readMask = 0;
        uint32_t  writeMask = 0;
        uint32_t  reference = 0;
    };

    struct StencilDescriptor
    {
        bool testEnabled = false;
        bool referenceDynamic = false;
        StencilFaceDescriptor front;
        StencilFaceDescriptor back;
    };

    struct DepthBiasDescriptor
    {
        float constantFactor = 0.0f;
        float slopeFactor = 0.0f;
        float clamp = 0.0f;
    };

    struct RasterizerDescriptor
    {
        PolygonMode         polygonMode = PolygonMode::Fill;
        CullMode            cullMode = CullMode::Disabled;
        DepthBiasDescriptor depthBias;
        float               lineWidth = 1.0f;

        bool frontCCW = false;
        bool discardEnabled = false;
        bool depthClampEnabled = false;
        bool scissorTestEnabled = false;
        bool multiSampleEnabled = false;
        bool antiAliasedLineEnabled = false;
        bool conservativeRasterization = false;
    };

    struct BlendTargetDescriptor
    {
        bool            blendEnabled = false;

        BlendOp         srcColor = BlendOp::SrcAlpha;
        BlendOp         dstColor = BlendOp::InvSrcAlpha;
        BlendArithmetic colorArithmetic = BlendArithmetic::Add;

        BlendOp         srcAlpha = BlendOp::SrcAlpha;
        BlendOp         dstAlpha = BlendOp::InvSrcAlpha;
        BlendArithmetic alphaArithmetic = BlendArithmetic::Add;

        Color4B         colorMask = { true, true, true, true };
    };

    struct BlendDescriptor
    {
        bool alphaToCoverageEnabled = false;
        bool independentBlendEnabled = false;

        uint32_t sampleMask = 0;
        LogicOp  logicOp = LogicOp::Disabled;
        Color4F  blendFactor = { 0.0f, 0.0f, 0.0f, 0.0f };
        bool     blendFactorDynamic = false;

        BlendTargetDescriptor targets[WANLIX_MAX_NUM_COLOR_ATTACHMENTS];
    };

    struct TessellationDescriptor
    {
        TessellationPartition   partition = TessellationPartition::Undefined;
        Format   indexFormat = Format::Undefined;
        uint32_t maxTessFactor = 64;
        bool     outputWindingCCW = false;
    };

    class PipelineLayout;
    class ShaderProgram;
    class RenderPass;

    struct PipelineDescriptor
    {
        virtual bool IsGraphics() const = 0;
    };

    struct GraphicsPipelineDescriptor : public PipelineDescriptor
    {
        PrimitiveTopology       primitiveTopology = PrimitiveTopology::TriangleList;
        std::vector<Viewport>   viewports;
        std::vector<Scissor>    scissors;

        std::shared_ptr<PipelineLayout> pipelineLayout;
        std::shared_ptr<ShaderProgram>  shaderProgram;
        std::shared_ptr<RenderPass>     renderPass;
        
        DepthDescriptor         depth;
        StencilDescriptor       stencil;
        RasterizerDescriptor    rasterizer;
        BlendDescriptor         blend;
        TessellationDescriptor  tessellation;

        virtual bool IsGraphics() const override final { return true; }
    };

    struct ComputePipelineDescriptor : public PipelineDescriptor
    {
        std::shared_ptr<PipelineLayout> pipelineLayout;
        std::shared_ptr<ShaderProgram>  shaderProgram;

        virtual bool IsGraphics() const override final { return false; }
    };
}
