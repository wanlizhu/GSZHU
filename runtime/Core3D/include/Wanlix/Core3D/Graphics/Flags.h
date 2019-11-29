#pragma once

#include <cstdint>

namespace Wanlix
{
    /* ----- CommandBuffer Flags ----- */

    enum class RenderConditionMode
    {
        Wait,                   //!< Wait until the occlusion query result is available, before conditional rendering begins.
        NoWait,                 //!< Do not wait until the occlusion query result is available, before conditional rendering begins.
        ByRegionWait,           //!< Similar to Wait, but the renderer may discard the results of commands for any framebuffer region that did not contribute to the occlusion query.
        ByRegionNoWait,         //!< Similar to NoWait, but the renderer may discard the results of commands for any framebuffer region that did not contribute to the occlusion query.
        WaitInverted,           //!< Same as Wait, but the condition is inverted.
        NoWaitInverted,         //!< Same as NoWait, but the condition is inverted.
        ByRegionWaitInverted,   //!< Same as ByRegionWait, but the condition is inverted.
        ByRegionNoWaitInverted, //!< Same as ByRegionNoWait, but the condition is inverted.
    };

    /* To be compatible with Direct3D renderers,
     * it is recommended to always use FrontAndBack.
     */
    enum class StencilFace
    {
        FrontAndBack, //! Refers to both the front and back face of primitives.
        Front,
        Back,
    };

    enum class PipelineBindPoint
    {
        Undefined,  //!< Automatically determine pipeline binding point for a resource heap.
        Graphics,   //!< Graphics pipeline binding point.
        Compute,    //!< Compute pipeline binding point.
    };

    struct CommandBufferFlags
    {
        enum
        {
            DeferredSubmit = (1 << 0),  //!< Will be submitted as a secondary command buffer.
            MultiSubmit = (1 << 1),  //!< Can be submitted multiple times.
        };
    };

    struct ClearFlags
    {
        enum
        {
            Color   = (1 << 0),  //!< Clears the color attachment.
            Depth   = (1 << 1),  //!< Clears the depth attachment.
            Stencil = (1 << 2),  //!< Clears the stencil attachment.

            ColorDepth   = (Color | Depth),   //!< Clears the color and depth attachments.
            DepthStencil = (Depth | Stencil), //!< Clears the depth and stencil attachments.
            All = (Color | Depth | Stencil),  //!< Clears the color, depth and stencil attachments.
        };
    };

    enum class OpenGLContextProfile
    {
        CompatibilityProfile,
        CoreProfile,
        ESProfile,
    };

    enum class ResourceType
    {
        Undefined,
        Buffer,
        Texture,
        Sampler,
        Shader,
    };

    struct BindFlags
    {
        enum
        {
            VertexBuffer           = (1 << 0),
            IndexBuffer            = (1 << 1),
            ConstantBuffer         = (1 << 2),
            StreamOutputBuffer     = (1 << 3),
            IndirectBuffer         = (1 << 4),
            Sampled                = (1 << 5),
            Storage                = (1 << 6),
            ColorAttachment        = (1 << 7),
            DepthStencilAttachment = (1 << 8),
            CombinedTextureSampler = (1 << 9),
        };
    };

    struct CPUAccessFlags
    {
        enum
        {
            Read  = (1 << 0),
            Write = (1 << 1),
            ReadWrite = (Read | Write),
        };
    };

    struct MiscFlags
    {
        enum
        {
            DynamicUsage  = (1 << 0),
            FixedSamples  = (1 << 1),
            GenerateMips  = (1 << 2),
            NoInitialData = (1 << 3),
            Append        = (1 << 4),
            Counter       = (1 << 5),
        };
    };

    enum class TextureType
    {
        Texture1D,          //!< 1-Dimensional texture.
        Texture2D,          //!< 2-Dimensional texture.
        Texture3D,          //!< 3-Dimensional texture.
        TextureCube,        //!< Cube texture.
        Texture1DArray,     //!< 1-Dimensional array texture.
        Texture2DArray,     //!< 2-Dimensional array texture.
        TextureCubeArray,   //!< Cube array texture.
        Texture2DMS,        //!< 2-Dimensional multi-sample texture.
        Texture2DMSArray,   //!< 2-Dimensional multi-sample array texture.
    };

    /* Can be used to change the order of texel components independently of a shader.*/
    enum class TextureSwizzle
    {
        Zero,   //!< The component is replaced by the constant zero.
        One,    //!< The component is replaced by the constant one.
        Red,    //!< The component is replaced by red component.
        Green,  //!< The component is replaced by green component.
        Blue,   //!< The component is replaced by blue component.
        Alpha   //!< The component is replaced by alpha component.
    };
}