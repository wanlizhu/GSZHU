#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace djv
{
#define DECL_DEVICE_OBJECT_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
    DECL_DEVICE_OBJECT_HANDLE(GPUBuffer);
    DECL_DEVICE_OBJECT_HANDLE(GPUBufferView);
    DECL_DEVICE_OBJECT_HANDLE(GPUImage);
    DECL_DEVICE_OBJECT_HANDLE(GPUImageView);
    DECL_DEVICE_OBJECT_HANDLE(GPUSampler);
    DECL_DEVICE_OBJECT_HANDLE(GPUShaderProgram);
    DECL_DEVICE_OBJECT_HANDLE(GPUPipeline);
    DECL_DEVICE_OBJECT_HANDLE(GPURenderPass);
    DECL_DEVICE_OBJECT_HANDLE(GPUFramebuffer);
    DECL_DEVICE_OBJECT_HANDLE(GPUCommandBuffer);
    DECL_DEVICE_OBJECT_HANDLE(GPUSwapchain);
#undef DECL_DEVICE_OBJECT_HANDLE

    /*
     * Combined with BufferUsage and ImageUsage into ResourceUsage
    */
    enum class ResourceUsage
    {
        /* Shared */
        Undefined = 0,
        TransferSRC,
        TransferDST,

        /* Buffer */
        VertexBuffer,
        IndexBuffer,
        IndirectBuffer,
        UniformBuffer,
        StorageBuffer,
        TransformFeedbackBuffer,
        TransformFeedbackCounterBuffer,

        /* Image */
        SampledImage,
        StorageImage,
        RenderTarget,
        DepthStencilTarget,
    };

    enum class MemoryProperty
    {
        Undefined = 0,
        DeviceLocal,
        DeviceCoherent,
        DeviceUncached,
        HostVisible,
        HostCoherent,
        HostCached,
    };

    enum class Format
    {
        Undefined = 0,
        R8_UINT,
        R8_SINT,
        R8G8_UINT,
        R8G8_SINT,
        R8G8B8_UINT,
        R8G8B8_SINT,
        R8G8B8A8_UINT,
        R8G8B8A8_SINT,
        R32_UINT,
        R32_SINT,
        R32_FLOAT,
        R32G32_UINT,
        R32G32_SINT,
        R32G32_FLOAT,
        R32G32B32_UINT,
        R32G32B32_SINT,
        R32G32B32_FLOAT,
        R32G32B32A32_UINT,
        R32G32B32A32_SINT,
        R32G32B32A32_FLOAT,
        S8_UINT,
        D24_UNORM_S8_UINT,
        D32_FLOAT,
        D32_FLOAT_S8_UINT,
    };

    enum class Filter
    {
        Nearest,
        Linear,
        CubicIMG
    };

    enum class MipmapMode
    {
        Nearest,
        Linear,
    };

    enum class AddressMode
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
        MirrorClampToEdge,
        MirrorClampToBorder,
    };

    enum class CompareOp
    {
        Never,
        Always,
        Less,
        Equal,
        NotEqual,
        LessOrEqual,
        Greater,
        GreaterOrEqual,
    };

    struct VertexInputState
    {};

    struct ViewportState
    {};

    struct RasterizationState 
    {};

    struct DepthStencilState 
    {};

    struct ColorBlendState
    {};

    struct AttachmentDescription
    {};
}