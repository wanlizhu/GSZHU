#pragma once

#include "Core/Common.h"

namespace Wanlix
{
    enum class ESupportedFeatures
    {
        None = 0,
        SamplePosPartial = 1,     // Allows one sample position to be set.
        SamplePosFull = (1 << 1), // Allows up to 4 sample positions to be set.
        Raytracing = (1 << 2),
    };
    ENUM_CLASS_OPERATORS(ESupportedFeatures)

    enum class EDataType
    {
        Undefined = 0,
        Int8,
        Int16,
        Int32,
        Int64,
        Uint8,
        Uint16,
        Uint32,
        Uint64,
        Float16,
        Float32,
        Float64,
    };
    ENUM_CLASS_OPERATORS(EDataType)

    enum class EBlendFactor
    {
        Undefined = 0,
        Zero,
        One,

        ColorSrc,
        ColorSrcInv,
        AlphaSrc,
        AlphaSrcInv,

        ColorDest,
        ColorDestInv,
        AlphaDest,
        AlphaDestInv,

        ConstColor,
        ConstColorInv,

        ColorSrc1,
        ColorSrc1Inv,
        AlphaSrc1,
        AlphaSrc1Inv,
    };

    enum class EBlendOp
    {
        Undefined = 0,
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };

    struct EColorMask
    {
        enum {
            Red = 1,
            Green = 2,
            Blue = 4,
            Alpha = 8,
            All = (Red | Green | Blue | Alpha),
        };
    };

    enum class ELogicOp
    {
        Clear = 0,
        Set,
        Copy,
        CopyInverted,
        NoOp,
        Invert,
        And,
        NAnd,
        Or,
        NOr,
        XOr,
        Equiv,
        AndReverse,
        OrReverse,
        OrInverted,
    };

    enum class EBufferMode
    {
        Undefined = 0,
        Formatted,
        Structured,
        Raw,
    };

    enum class EBindFlags 
    {
        None = 0,
        VertexBuffer = 1,
        IndexBuffer = (1 << 1),
        UniformBuffer = (1 << 2),
        ShaderResource = (1 << 3),
        StreamOutput = (1 << 4),
        RenderTarget = (1 << 5),
        DepthStencil = (1 << 6),
        UnorderedAccess = (1 << 7),
        IndirectDrawArguments = (1 << 8),
    };
    ENUM_CLASS_OPERATORS(EBindFlags)

    enum class ECPUAccessFlags
    {
        None,
        Read,
        Write,
    };
    ENUM_CLASS_OPERATORS(ECPUAccessFlags)

    enum class EMapType
    {
        Read = 1,
        Write = 2,
        ReadWrite = 3,
    };
    ENUM_CLASS_OPERATORS(EMapType)

    enum class EMapFlags
    {
        None = 0,
        DoNotWait = 1,
        Discard = (1 << 1),
        DoNotWaitSync = (1 << 2),
    };
    ENUM_CLASS_OPERATORS(EMapFlags)

    enum class EResourceState
    {
        Undefined = 0,
        Unknown,
        PreInitialized,
        Common,
        VertexBuffer,
        ConstantBuffer,
        IndexBuffer,
        RenderTarget,
        UnorderedAccess,
        DepthStencil,
        ShaderResource,
        StreamOut,
        IndirectArg,
        CopyDest,
        CopySource,
        ResolveDest,
        ResolveSource,
        Present,
        GenericRead,
        Predication,
        NonPixelShader,
        AccelerationStructure,
    };

    enum class EResourceType
    {
        Undefined = 0,
        Sampler,
        Buffer,
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        Texture3D,
        TextureCube,
        TextureCubeArray,
        Texture2DMultisample,
    };

    enum class EUsage
    {
        Default = 0,
        Static,
        Dynamic,
        Staging,
    };

    enum class EMiscFlags
    {
        None = 0,
        GenerateMips = 1,
    };

    enum class EBufferViewType
    {
        Undefined = 0,
        ShaderResource,
        UnorderedAccess,

        ShaderRead = ShaderResource,
        ShaderReadWrite = UnorderedAccess,
    };

    enum class ETextureViewType
    {
        Undefined = 0,
        ShaderResource,
        RenderTarget,
        DepthStencil,
        UnorderedAccess,

        ShaderRead = ShaderResource,
        ShaderReadWrite = UnorderedAccess,
    };

    enum class EUAVAccessFlags
    {
        Undefined = 0,
        Read = 1,
        Write = (1 << 1),
        ReadWrite = Read | Write,
    };
    ENUM_CLASS_OPERATORS(EUAVAccessFlags)

    enum class ETextureViewFlags
    {
        None = 0,
        AllowGenerateMip = 1,
    };
    ENUM_CLASS_OPERATORS(ETextureViewFlags)

    enum class EStencilOp
    {
        Undefined,
        Keep,
        Zero,
        Replace,
        IncrSat,
        DecrSat,
        Invert,
        IncrWrap,
        DecrWrap,
    };

    enum class EComparisonFunc
    {
        Unknown,
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always,
    };

    enum class EDeviceType
    {
        Undefined,
        D3D12,
        OpenGL,
        OpenGLES,
        Vulkan,
        Metal,
    };

    enum class ECommandQueue
    {
        Copy = 0,
        Compute,
        Graphics,
        Count,
    };

    enum class EDrawFlags
    {
        None,
        VerifyStates,
        VerifyParameters,
        VerifyRenderTargets,
        VerifyAll = VerifyStates | VerifyParameters | VerifyRenderTargets,
    };
    ENUM_CLASS_OPERATORS(EDrawFlags)

    enum class EClearFlags
    {
        None = 0,
        Color,
        Depth,
        Stencil,
        ColorDepth = Color | Depth,
        DepthStencil = Depth | Stencil,
        All = Color | Depth | Stencil,
    };
    ENUM_CLASS_OPERATORS(EClearFlags)

    enum class EInputFrequency
    {
        Undefined = 0,
        PerVertex,
        PerInstance,
    };

    enum class EShaderVariableType
    {
        Static = 0,
        Mutable,
        Dynamic,
    };

    enum class EPrimitiveTopology
    {
        Undefined = 0,
        TriangleList,
        TriangleList_Adjacency,
        TriangleStrip,
        TriangleStrip_Adjacency,
        PointList,
        LineList,
        LineList_Adjacency,
        LineStrip,
        LineStrip_Adjacency,
        PatchList,
    };

    enum class EShaderStages
    {
        Unknown = 0,
        Vertex,
        Fragment,
        Geometry,
        TessEval,
        TessCtrl,
        Compute,

        Basic = Vertex | Fragment,
        All = Vertex | Fragment | Geometry | TessEval | TessCtrl | Compute,
    };
    ENUM_CLASS_OPERATORS(EShaderStages)

    enum class EBindResourceFlags
    {
        None = 0,
        UpdateStatic,
        UpdateMutable,
        UpdateDynamic,
        UpdateAll,
        KeepExisting,
        ShouldAllResolved,
    };
    ENUM_CLASS_OPERATORS(EBindResourceFlags)

    enum class EFillMode
    {
        Undefined = 0,
        Wireframe,
        Solid,
    };

    enum class ECullMode
    {
        Undefined = 0,
        None,
        Front,
        Back,
    };

    enum class EFilterType
    {
        Unknown = 0,
        Point,
        Linear,
        Anisotropic,
        ComparisonPoint,
        ComparisonLinear,
        ComparisonAnisotropic,
        MinPoint,
        MinLinear,
        MinAnisotropic,
        MaxPoint,
        MaxLinear,
        MaxAnisotropic,
    };

    enum class EAddressMode
    {
        Unknown = 0,
        Wrap,
        Mirror,
        Clamp,
        Border,
        MirrorOnce,
    };

    enum class EShaderSource
    {
        None = 0,
        SourceFile,
        SourceCode,
        ByteCode,
    };

    enum class EShaderType
    {
        Unknown = 0,
        Vertex,
        Fragment,
        Geometry,
        TessEval,
        TessCtrl,
        Compute,
    };

    enum class EShaderProfile
    {
        Default = 0,
        Dx_5_0,
        Dx_5_1,
        Gl_4_2,
    };

    enum class EShaderLanguage
    {
        Default = 0,
        Hlsl,
        Glsl,
        SpirV,
    };

    enum class EShaderResourceType
    {
        Unknown = 0,
        UniformBuffer,
        TextureSRV,
        BufferSRV,
        TextureUAV,
        BufferUAV,
        Sampler,
    };

    enum class EShaderCompileFlags
    {
        None = 0,
        TreatWarningsAsErrors = 1,
        DumpIntermediates = (1 << 1),
        FloatingPointModeFast = (1 << 2),
        FloatingPointModePrecise = (1 << 3),
        GenerateDebugInfo = (1 << 4),
    };

    enum class EScaling
    {
        Unspecified = 0,
        Centered,
        Stretched,
    };

    enum class EScanLineOrder
    {
        Unspecified = 0,
        Progressive,
        UpperFieldFirst,
        LowerFieldFirst,
    };
}