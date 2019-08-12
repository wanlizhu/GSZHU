#pragma once

#include <stdint.h>

namespace ZHU
{
    template<typename T>
    constexpr auto conv(T e) 
    {
        return static_cast<typename std::underlying_type_t<T>>(e);
    }


    enum class EComponent : uint8_t
    {
        Undefined = 0,
        INT8,
        INT16,
        INT32,

        UINT8,
        UINT16,
        UINT32,

        FLOAT16,
        FLOAT32,
        FLOAT64
    };

    enum class EBufferMode : uint8_t
    {
        Undefined = 0,
        // ElementByteStride member of BufferDesc defines the buffer element size,
        // Buffer views can use different formats, but the format size must match ElementByteStride.
        Formatted, 
        // ElementByteStride member of BufferDesc defines the structure stride.
        Structured,
        Raw
    };


    enum class EBind : uint32_t
    {
        None = 0x0,
        VertexBuffer = 0x1,
        IndexBuffer = 0x2,
        UniformBuffer = 0x4,
        ShaderResource = 0x8,
        StreamOutput = 0x10,
        RenderTarget = 0x20,
        DepthStencil = 0x40,
        UnorderedAccess = 0x80,
        IndirectDraw = 0x100
    };


    enum class EUsage : uint8_t
    {
        // A resource that can only be read by the GPU. It cannot be written by the GPU, 
        // and cannot be accessed at all by the CPU. This type of resource must be initialized 
        // when it is created, since it cannot be changed after creation.
        Static,
        // A resource that requires read and write access by the GPU and can also be occasionally
        // written by the CPU.
        Default,
        // A resource that can be read by the GPU and written at least once per frame by the CPU.
        Dynamic,
        // A resource that facilitates transferring data from GPU to CPU.
        CPUAccessible
    };


    // Only USAGE_DYNAMIC resources can be mapped
    enum class EMappedCPUAccess : uint8_t
    {
        None = 0,
        Read = 1, // A resource can be mapped for reading
        Write = 2 // A resource can be mapped for writing
    };


    enum class EUAVAccess : uint8_t
    {
        Undefined = 0,
        Read = 1,
        Write = 2,
        Read_Write = conv(EUAVAccess::Read) | conv(EUAVAccess::Write)
    };


    enum class EBufferView : uint8_t
    {
        Undefined = 0,
        ShaderResource,  // will be used as the source for the shader READ operations
        UnorderedAccess, // will be used as the source for the shader READ/WRITE operations
    };


    enum class ETextureView : uint8_t
    {
        Undefined = 0,
        ShaderResource,
        RenderTarget,
        DepthStencil,
        UnorderedAccess
    };


    enum class EResourceState : uint32_t
    {
        Undefined = 0x0,
        Unknown = 0x1,
        VertexBuffer = 0x2,
        IndexBuffer = 0x4,
        UniformBuffer = 0x8,
        RenderTarget = 0x10,
        UnorderedAccess = 0x20,
        DepthWrite = 0x40,
        DepthRead = 0x80,
        ShaderResource = 0x100,
        StreamOut = 0x200,
        IndirectDraw = 0x400,

        CopySource = 0x800,
        CopyDestination = 0x1000,
        ResolveSource = 0x2000,
        ResolveDestination = 0x4000,

        Present = 0x8000,

        GenericRead = conv(EResourceState::VertexBuffer) |
                      conv(EResourceState::IndexBuffer) |
                      conv(EResourceState::UniformBuffer) |
                      conv(EResourceState::ShaderResource) |
                      conv(EResourceState::IndirectDraw) |
                      conv(EResourceState::CopySource)
    };


    // On D3D11, comparison filters only work with textures that have the following formats:
    //        R32_FLOAT_X8X24_TYPELESS, R32_FLOAT, R24_UNORM_X8_TYPELESS, R16_UNORM.
    enum class EFilter : uint8_t
    {
       Unknown = 0,
       Point,
       Linear,
       Anisotropic,
       ComparisonPoint,
       ComparisonLinear,
       ComparisonAnisotropic,
       MinimumPoint,
       MinimumLinear,
       MinimumAnisotropic,
       MaximumPoint,
       MaximumLinear,
       MaximumAnisotropic
    };


    enum class EAddressMode : uint8_t
    {
        Unknown = 0,
        Warp,
        Mirror,
        MirrorOnce, // OpenGL counterpart : GL_MIRROR_CLAMP_TO_EDGE
        Clamp,
        Border
    };


    enum class EComparisonFunction : uint8_t
    {
        Unknown = 0,
        Never,
        Always,

        Equal,
        NotEqual,

        Less,
        LessEqual,

        Greater,
        GreaterEqual,
    };


    enum class ETextureDimension : uint8_t
    {
        Undefined = 0,
        Buffer,
        Tex1D,
        Tex1DArray,
        Tex2D,
        Tex2DArray,
        Tex3D,
        Cube,
        CubeArray
    };


    enum class EPixelFormat : uint16_t
    {
        Unknown,

        RGBA32F,
        RGBA32UI,
        RGBA32I,

        RGB32F,
        RGB32UI,
        RGB32I,

        RGBA16F,
        RGBA16,
        RGBA16UI,
        RGBA16I,

        RG32F,
        RG32UI,
        RG32I,

        DEPTH32F_STENCIL8,

        RGB10_A2,
        RGB10_A2UI,
        R11F_G11F_B10F,

        RGBA8,
        RGBA8_SRGB,
        RGBA8UI,
        RGBA8I,

        RG16F,
        RG16UI,
        RG16I,

        R32F,
        DEPTH32F,
        R32UI,
        R32I,

        DEPTH24_STENCIL8,

        RG8,
        RG8UI,
        RG8I,

        R16F,
        DEPTH16,
        R16UI,
        R16I,

        R8,
        R8UI,
        R8I,
    };


    enum class EDisplayScaling : uint8_t
    {
         Undefined = 0,
         Centered = 1,
         Stretched = 2
    };


    enum class EDisplayOrder : uint8_t
    {
        Undefined = 0,
        Progressive = 1,
        UpperFieldFirst = 2,
        LowerFieldFirst = 4
    };


    enum class EColorMask : uint8_t
    {
        Red = 1,
        Green = 2,
        Blue = 4,
        Alpha = 8,
        All = conv(EColorMask::Red) | 
              conv(EColorMask::Green) |
              conv(EColorMask::Blue) |
              conv(EColorMask::Alpha)
    };


    enum class EBlendFactor : uint8_t
    {
        Undefined = 0,
        Zero,
        One,
        SourceColor,
        One_Minus_SourceColor,
        SourceAlpha,
        One_Minus_SourceAlpha,
        DestColor,
        One_Minus_DestColor,
        DestAlpha,
        One_Minus_DestAlpha,
        ConstantColor,
        One_Minus_ConstantColor
    };


    enum class EBlendOperation : uint8_t
    {
        Undefined = 0,
        Add,
        Subtract,
        Min,
        Max
    };


    enum class EStencilOperation : uint8_t
    {
        Undefined = 0,
        Keep,
        Zero,
        Replace,
        Increment,
        IncrementWrap,
        DecrementWrap,
        Invert
    };


    enum class EDevice : uint32_t
    {
        Undefined = 0,
        D3D11,
        D3D12,
        OpenGL,
        Vulkan
    };


    enum class EFrequency : uint8_t
    {
        Undefined = 0,
        Vertex,
        Instance
    };


    enum class EFillMode : int8_t
    {
        Undefined = 0,
        Wireframe,
        Solid
    };


    enum class ECullMode : uint8_t
    {
        Undefined = 0,
        None,
        Front,
        Back
    };


    enum class EPrimitiveTopology : uint8_t
    {
        Undefined = 0,
        PointList,

        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,

        LineStripAdjacency,
        TriangleListAdjacency,
        LineListAdjacency,
        TriangleStripAdjacency,

        TriangleFan,
        PatchList
    };
}