#pragma once

#include <GSZHU/ENUM/EStencilOperation.h>
#include <GSZHU/ENUM/EComparisonFunction.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    // It is used by DepthStencilStateDesc to describe the stencil operations for the front and back facing polygons.
    struct SStencilOpDesc {
        ESTENCIL_OP StencilFailOp = STENCIL_OP_KEEP;
        ESTENCIL_OP StencilDepthFailOp = STENCIL_OP_KEEP;
        ESTENCIL_OP StencilPassOp = STENCIL_OP_KEEP;
        ECOMPARISON_FUNCTION StencilFunc = COMPARISON_FUNC_ALWAYS;

        SStencilOpDesc() noexcept;
        SStencilOpDesc(ESTENCIL_OP _StencilFailOp,
                       ESTENCIL_OP _StencilDepthFailOp,
                       ESTENCIL_OP _StencilPassOp,
                       ECOMPARISON_FUNCTION _StencilFunc) noexcept;

        bool operator==(const SStencilOpDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SStencilOpDesc> {
        size_t operator()(const GSZHU::SStencilOpDesc& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.StencilFailOp),
                                      static_cast<int>(Desc.StencilDepthFailOp),
                                      static_cast<int>(Desc.StencilPassOp),
                                      static_cast<int>(Desc.StencilFunc));
        }
    };
}