#pragma once

#include <GSZHU/ENUM/EBlendFactor.h>
#include <GSZHU/ENUM/EBlendOperation.h>
#include <GSZHU/ENUM/EColorMask.h>
#include <GSZHU/ENUM/ELogicOperation.h>
#include <GSZHU/BasicTools.h>

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
        uint8_t RenderTargetWriteMask = COLOR_MASK_ALL;

        SRenderTargetBlendDesc() noexcept;
        explicit SRenderTargetBlendDesc(bool _BlendEnable) noexcept;

        bool operator==(const SRenderTargetBlendDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SRenderTargetBlendDesc> {
        size_t operator()(const GSZHU::SRenderTargetBlendDesc& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.BlendEnable),
                                      static_cast<int>(Desc.LogicOperationEnable),
                                      static_cast<int>(Desc.SrcBlend),
                                      static_cast<int>(Desc.DestBlend),
                                      static_cast<int>(Desc.BlendOp),
                                      static_cast<int>(Desc.SrcBlendAlpha),
                                      static_cast<int>(Desc.DestBlendAlpha),
                                      static_cast<int>(Desc.BlendOpAlpha),
                                      static_cast<int>(Desc.LogicOp),
                                      static_cast<int>(Desc.RenderTargetWriteMask));
        }
    };
}