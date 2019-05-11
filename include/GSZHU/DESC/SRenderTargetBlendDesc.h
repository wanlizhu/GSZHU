#pragma once

#include <GSZHU/ENUM/EBlendFactor.h>
#include <GSZHU/ENUM/EBlendOperation.h>
#include <GSZHU/ENUM/EColorMask.h>
#include <GSZHU/ENUM/ELogicOperation.h>

namespace GSZHU {
    // This structure is used by BlendStateDesc to describe blend states for render targets
    struct SRenderTargetBlendDesc {
        bool BlendEnable = false;
        bool LogicOperationEnable = false;

        EBLEND_FACTOR SrcBlend = BLEND_FACTOR_ONE;
        EBLEND_FACTOR DestBlend = BLEND_FACTOR_ZERO;
        EBLEND_OP BlendOp = BLEND_OP_ADD;

        EBLEND_FACTOR SrcBlendAlpha = BLEND_FACTOR_ONE;
        EBLEND_FACTOR DestBlendAlpha = BLEND_FACTOR_ZERO;
        EBLEND_OP BlendOpAlpha = BLEND_OP_ADD;

        ELOGIC_OP LogicOp = LOGIC_OP_NOOP;
        UINT8 RenderTargetWriteMask = COLOR_MASK_ALL;

        SRenderTargetBlendDesc() noexcept {}
        explicit SRenderTargetBlendDesc(bool _BlendEnable) noexcept
            : BlendEnable(_BlendEnable)
        {}

        bool operator==(const SRenderTargetBlendDesc& rhs) const {
            return BlendEnable == rhs.BlendEnable
                && LogicOperationEnable == rhs.LogicOperationEnable
                && SrcBlend == rhs.SrcBlend
                && DestBlend == rhs.DestBlend
                && BlendOp == rhs.BlendOp
                && SrcBlendAlpha == rhs.SrcBlendAlpha
                && DestBlendAlpha == rhs.DestBlendAlpha
                && BlendOpAlpha == rhs.BlendOpAlpha
                && LogicOp == rhs.LogicOp
                && RenderTargetWriteMask == rhs.RenderTargetWriteMask;
        }
    };
}