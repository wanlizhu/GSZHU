#pragma once

#include "Graphics/Config.h"
#include <stdint.h>
#include <type_traits>

namespace ZHU
{
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
        // Buffer views can use different formats, but the format size must match ElementByteStride.
        Formatted, 
        // ElementByteStride member of BufferDesc defines the structure stride.
        Structured,
        Raw
    };


    enum class ECanBindAs : uint32_t
    {
        None = 0x0,
        VertexBuffer = 0x1,
        IndexBuffer = 0x2,
        UniformBuffer = 0x4,
		// This flag may not be combined with any other bind flag
        ShaderResource = 0x8, 
		// This flag cannot be used with MAP_WRITE_NO_OVERWRITE flag
        StreamOutput = 0x10,
        RenderTarget = 0x20,
        DepthStencil = 0x40,
        UnorderedAccess = 0x80,
        IndirectDrawArgs = 0x100
    };
	DEFINE_ENUM_OPERATORS(ECanBindAs);


    enum class EUsage : uint8_t
    {
		// Can be read by GPU, cannot be written by GPU, and cannot be accessed at all by CPU
        Static = 0,
        // A resource that requires read and write access by the GPU and can also be occasionally written by the CPU.
        Default,
        // A resource that can be read by the GPU and written at least once per frame by the CPU.
        Dynamic,
        // A resource that facilitates transferring data from GPU to CPU.
        Staging
    };


    enum class EAccess : uint8_t
    {
        None = 0,
        Read = 1, // A resource can be mapped for reading
        Write = 2 // A resource can be mapped for writing
    };
	DEFINE_ENUM_OPERATORS(EAccess);


	// Describes how a mapped resource will be accessed.
	enum class EMap : uint8_t
	{
		Read = 1,
		Write = 2,
		ReadWrite = 3
    };


	enum class EMapFlags : uint8_t
	{
		None = 0,
		// Specifies that map operation should not wait until previous command that
		// using the same resource completes. Map returns null pointer if the resource
		// is still in use
		// OpenGL does not have corresponding flag.
		DoNotWait = 1,
		// Previous contents of the resource will be undefined. This flag is only compatible with MAP_WRITE
		// OpenGL counterpart: GL_MAP_INVALIDATE_BUFFER_BIT
		Discard = 2,
		// The system will not synchronize pending operations before mapping the buffer.
		// It is responsibility of the application to make sure that the buffer contents 
		// is not overwritten while it is in use by the GPU.
		// OpenGL counterpart: GL_MAP_UNSYNCHRONIZED_BIT
		DoNotSynchronize = 4
	};
	DEFINE_ENUM_OPERATORS(EMapFlags);


	enum class EResourceDimension : uint8_t
	{
		Undefined = 0,
		Buffer,
		Tex1D,
		Tex1DArray,
		Tex2D,
		Tex2DArray,
		Tex3D,
		TexCube,
		TexCubeArray
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
        IndirectDrawArgs = 0x400,

        CopySource = 0x800,
        CopyDestination = 0x1000,
        ResolveSource = 0x2000,
        ResolveDestination = 0x4000,

        Present = 0x8000,

        GenericRead = (UnderType<EResourceState>)EResourceState::VertexBuffer |
                      (UnderType<EResourceState>)EResourceState::IndexBuffer |
                      (UnderType<EResourceState>)EResourceState::UniformBuffer |
                      (UnderType<EResourceState>)EResourceState::ShaderResource |
                      (UnderType<EResourceState>)EResourceState::IndirectDrawArgs |
                      (UnderType<EResourceState>)EResourceState::CopySource
    };
	DEFINE_ENUM_OPERATORS(EResourceState);


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


    enum class ETextureFormat : uint16_t
    {
        Unknown,

        RGBA32F,
        RGBA32UI,
        RGBA32I,

        RGB32F,
        RGB32UI,
        RGB32I,

        RGBA16F,
        RGBA16UN,
        RGBA16UI,
		RGBA16SN,
        RGBA16I,

        RG32F,
        RG32UI,
        RG32I,

        DEPTH32F_STENCIL8,

        RGB10_A2UN,
        RGB10_A2UI,
        RG11_B10F,

        RGBA8UN,
        RGBA8UN_SRGB,
        RGBA8UI,
		RGBA8SN,
        RGBA8I,

        RG16F,
		RG16UN,
        RG16UI,
		RG16SN,
        RG16I,

		DEPTH32F,
        R32F,
        R32UI,
        R32I,

        DEPTH24UN_STENCIL8,

        RG8UN,
        RG8UI,
		RG8SN,
        RG8I,

        R16F,
        DEPTH16UN,
		R16UN,
        R16UI,
		R16SN,
        R16I,

        R8UN,
        R8UI,
		R8SN,
        R8I,

		RGB9E5_SHAREDEXP,

		BC1,
		BC1_UN,
		BC1_UN_SRGB,
		BC2,
		BC2_UN,
		BC2_UN_SRGB,
		BC3,
		BC3_UN,
		BC3_UN_SRGB,
		BC4,
		BC4_UN,
		BC4_SN,
		BC5,
		BC5_UN,
		BC5_SN,
		BC6H,
		BC6H_UF16,
		BC6H_SF16,
		BC7,
		BC7_UN,
		BC7_UN_SRGB
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
        All = (UnderType<EColorMask>)EColorMask::Red | 
              (UnderType<EColorMask>)EColorMask::Green |
              (UnderType<EColorMask>)EColorMask::Blue |
              (UnderType<EColorMask>)EColorMask::Alpha
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


	enum class ESwapChainUsage : uint8_t
	{
		None = 0,
		RenderTarget = 1,
		ShaderInput = 2,
		CopySource = 4
	};
	DEFINE_ENUM_OPERATORS(ESwapChainUsage);
}