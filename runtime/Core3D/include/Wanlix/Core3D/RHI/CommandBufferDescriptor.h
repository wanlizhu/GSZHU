#pragma once

#include "Color.h"


namespace Wanlix
{
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

    // To be compatible with Direct3D renderers, it is recommended to always use FrontAndBack.
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
        uint32_t value = 0;
        enum
        {
            // Specifies that the encoded command buffer will be submitted as a secondary command buffer.
            DeferredSubmit = (1 << 0),
            // Specifies that the encoded command buffer can be submitted multiple times.
            MultiSubmit = (1 << 1),
        };

        CommandBufferFlags(uint32_t val = 0) : value(val) {}
        operator uint32_t() const { return value; }
        DEFINE_CMP_OPS(CommandBufferFlags, value)
        DEFINE_BIT_OPS(uint32_t, value)
    };

    struct ClearFlags
    {
        uint32_t value = 0;
        enum
        {
            Color = (1 << 0),                  //!< Clears the color attachment.
            Depth = (1 << 1),                  //!< Clears the depth attachment.
            Stencil = (1 << 2),                //!< Clears the stencil attachment.

            ColorDepth = (Color | Depth),      //!< Clears the color and depth attachments.
            DepthStencil = (Depth | Stencil),  //!< Clears the depth and stencil attachments.
            All = (Color | Depth | Stencil),   //!< Clears the color, depth and stencil attachments.
        };

        ClearFlags(uint32_t val = 0) : value(val) {}
        operator uint32_t() const { return value; }
        DEFINE_CMP_OPS(ClearFlags, value)
        DEFINE_BIT_OPS(uint32_t, value)
    };

    struct ClearValue
    {
        ColorRGBA<float> color = { 0.0f, 0.0f, 0.0f, 0.0f };
        float depth = 1.0f;
        uint32_t stencil = 0;
    };

    struct AttachmentClear
    {
        ClearFlags flags = 0;
        uint32_t colorAttachment = 0;
        ClearValue clearValue;


        AttachmentClear() = default;
        AttachmentClear(
            const ColorRGBA<float>& color,
            uint32_t index
        )
            : flags(ClearFlags::Color)
            , colorAttachment(index)
        {
            clearValue.color = color;
        }

        AttachmentClear(
            float depth,
            uint32_t stencil
        )
            : flags(ClearFlags::DepthStencil)
        {
            clearValue.depth = depth;
            clearValue.stencil = stencil;
        }

        AttachmentClear(float depth)
            : flags(ClearFlags::Depth)
        {
            clearValue.depth = depth;
        }

        AttachmentClear(uint32_t stencil)
            : flags(ClearFlags::Stencil)
        {
            clearValue.stencil = stencil;
        }
    };

    struct OpenGLDescriptor
    {
        bool originLowerLeft = false; // Upper-left is the default of Direct3D and Vulkan.
        bool invertFrontFace = false;
    };

    struct MetalDescriptor
    {
        // Specifies the buffer slot for the internal tessellation factor buffer. 
        // By default 30, which is the maximum buffer slot.
        // In the respective Metal tessellation kernel, 
        // this must refer to a buffer of type MTLTriangleTessellationFactorsHalf or MTLQuadTessellationFactorsHalf.
        uint32_t tessFactorBufferSlot = 30;
    };

    struct CommandBufferDescriptor
    {
        CommandBufferFlags flags = 0;
        // this member specifies how many native command buffers are to be allocated internally.
        // These native command buffers are then switched everytime encoding begins with the CommandBuffer::Begin function.
        // The benefit of having multiple native command buffers is that it reduces the time the GPU is idle
        // because it waits for a command buffer to be completed before it can be reused.
        uint32_t numNativeBuffers = 2;
    };
}