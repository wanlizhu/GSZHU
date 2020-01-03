#pragma once

#include <cstdint>

namespace Wanlix
{
    enum class DebugSeverity
    {
        Info = 0,
        Warning,
        Error,
        FatalError,
    };

    enum class BlendFactor
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

    enum class BlendOp
    {
        Undefined = 0,
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };

    struct ColorMask
    {
        enum {
            Red = 1,
            Green = 2,
            Blue = 4,
            Alpha = 8,
            All = (Red | Green | Blue | Alpha),
        };
    };

    enum class LogicOp
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

    enum class BufferMode
    {
        Undefined = 0,
        Formatted,
        Structured,
        Raw,
    };

    struct BindFlags
    {
        enum {
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
    };

    struct CpuAccessFlags
    {
        enum {
            None,
            Read,
            Write,
        };
    };

    struct MapType
    {
        enum {
            Read = 1,
            Write = 2,
            ReadWrite = 3,
        };
    };

    struct MapFlags
    {
        enum {
            None = 0,
            DoNotWait = 1,
            Discard = (1 << 1),
            DoNotWaitSync = (1 << 2),
        };
    };

    enum class ResourceState
    {
        Undefined,
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

    enum class TransitionBarrierType
    {
        Immediate = 0,
        Begin, // This mode only has effect in Direct3D12 backend
        End,   // This mode only has effect in Direct3D12 backend
    };

    enum class ResourceType
    {
        Undefined = 0,
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

    enum class Usage
    {
        Default = 0,
        Static,
        Dynamic,
        Staging,
    };

    struct MiscFlags
    {
        enum {
            None = 0,
            GenerateMips = 1,
        };
    };

    enum class BufferViewType
    {
        Undefined = 0,
        ShaderResource,
        UnorderedAccess,

        ShaderRead = ShaderResource,
        ShaderReadWrite = UnorderedAccess,
    };

    enum class TextureViewType
    {
        Undefined = 0,
        ShaderResource,
        RenderTarget,
        DepthStencil,
        UnorderedAccess,

        ShaderRead = ShaderResource,
        ShaderReadWrite = UnorderedAccess,
    };

    struct UavAccessFlags
    {
        enum {
            Undefined = 0,
            Read = 1,
            Write = (1 << 1),
            ReadWrite = Read | Write,
        };
    };

    struct TextureViewFlags
    {
        enum {
            None = 0,
            AllowGenerateMip = 1,
        };
    };

    enum class StencilOp
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

    enum class ComparisonFunc
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

    enum class DeviceType
    {
        Undefined,
        D3D12,
        OpenGL,
        OpenGLES,
        Vulkan,
        Metal,
    };

    struct DrawFalgs
    {
        enum {
            None,
            VerifyStates,
            VerifyParameters,
            VerifyRenderTargets,
            VerifyAll = VerifyStates | VerifyParameters | VerifyRenderTargets,
        };
    };

    /* Resource state transition mode*/
    enum class RSTMode
    {
        None,
        // Any method that uses this mode may alter the state of the resources it works with.
        Transition,
        // Any method that uses this mode will read the state of resources it works with.
        Verify,
    };

    struct ClearFlags
    {
        enum {
            None = 0,
            Color,
            Depth,
            Stencil,
            ColorDepth = Color | Depth,
            DepthStencil = Depth | Stencil,
            All = Color | Depth | Stencil,
        };
    };

    struct SetVbsFlags
    {
        enum {
            None = 0,
            Reset,
        };
    };

    enum class InputFrequency
    {
        Undefined = 0,
        PerVertex,
        PerInstance,
    };

    enum class ShaderVariableType
    {
        Static = 0,
        Mutable,
        Dynamic,
    };

    enum class PrimitiveTopology
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

    struct ShaderStages
    {
        enum {
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
    };

    enum class FillMode
    {
        Undefined = 0,
        Wireframe,
        Solid,
    };

    enum class CullMode
    {
        Undefined = 0,
        None,
        Front,
        Back,
    };

    enum class FilterType
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

    enum class AddressMode
    {
        Unknown = 0,
        Wrap,
        Mirror,
        Clamp,
        Border,
        MirrorOnce,
    };

    enum class ShaderType
    {
        Unknown = 0,
        Vertex,
        Fragment,
        Geometry,
        TessEval,
        TessCtrl,
        Compute,
    };

    enum class ShaderProfile
    {
        Default = 0,
        Dx_5_0,
        Dx_5_1,
        Gl_4_2,
    };

    enum class ShaderLanguage
    {
        Default = 0,
        Hlsl,
        Glsl,
        SpirV,
    };

    enum class ShaderResourceType
    {
        Unknown = 0,
        UniformBuffer,
        TextureSRV,
        BufferSRV,
        TextureUAV,
        BufferUAV,
        Sampler,
    };

    struct SwapChainUsage
    {
        enum {
            None = 0,
            RenderTarget = 1,
            ShaderInput = (1 << 1),
            CopySource = (1 << 2),
            CopyDest = (1 << 3),
        };
    };

    enum class ScalingMode
    {
        Unspecified = 0,
        centered,
        Stretched,
    };

    enum class ScanLineMode
    {
        Unspecified = 0,
        Progressive,
        UpperFieldFirst,
        LowerFieldFirst,
    };
}