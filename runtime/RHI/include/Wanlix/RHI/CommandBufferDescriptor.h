#pragma once

#include "Common.h"

namespace Wanlix
{
    enum class RenderConditionMode
    {
        Wait,                   
        NoWait,                 
        ByRegionWait,           
        ByRegionNoWait,         
        WaitInverted,           
        NoWaitInverted,         
        ByRegionWaitInverted,   
        ByRegionNoWaitInverted, 
    };

    enum class StencilFace
    {
        FrontAndBack,
        Front,
        Back,
    };

    enum class PipelineBindPoint
    {
        Undefined,  
        Graphics,   
        Compute,    
    };

    struct CommandBufferFlags
    {
        enum
        {
            DeferredSubmit = (1 << 0),
            MultiSubmit = (1 << 1),
        };
    };

    struct ClearFlags
    {
        enum
        {
            Color = (1 << 0),                   
            Depth = (1 << 1),                   
            Stencil = (1 << 2),                 

            ColorDepth = (Color | Depth),       
            DepthStencil = (Depth | Stencil),   
            All = (Color | Depth | Stencil),    
        };
    };

    struct ClearValue
    {
        Color    color = { 0.0f, 0.0f, 0.0f, 0.0f };
        float    depth = 1.0f;
        uint32_t stencil = 0;

        ClearValue() = default;
        ClearValue(const Color& color) : color(color) {}
        ClearValue(float depth) : depth(depth) {}
        ClearValue(uint32_t stencil) : stencil(stencil) {}
        ClearValue(float depth, uint32_t stencil) : depth(depth), stencil(stencil) {}
    };

    struct AttachmentClear
    {
        long       flags = 0;
        uint32_t   colorAttachment = 0;
        ClearValue clearValue;

        AttachmentClear() = default;
        AttachmentClear(const Color& color, uint32_t colorAttachment)
            : flags(ClearFlags::Color)
            , colorAttachment(colorAttachment)
            , clearValue(color)
        {}
        AttachmentClear(float depth)
            : flags(ClearFlags::Depth)
            , clearValue(depth)
        {}
        AttachmentClear(uint32_t stencil)
            : flags(ClearFlags::Stencil)
            , clearValue(stencil)
        {}
        AttachmentClear(float depth, uint32_t stencil)
            : flags(ClearFlags::DepthStencil)
            , clearValue(depth, stencil)
        {}
    };
}