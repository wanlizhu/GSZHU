#pragma once

#include <GSZHU/ENUM/EStencilOperation.h>
#include <GSZHU/ENUM/EComparisonFunction.h>

namespace GSZHU {
    // It is used by DepthStencilStateDesc to describe the stencil operations for the front and back facing polygons.
    struct SStencilOpDesc {
        ESTENCIL_OP StencilFailOp = STENCIL_OP_KEEP;
        ESTENCIL_OP StencilDepthFailOp = STENCIL_OP_KEEP;
        ESTENCIL_OP StencilPassOp = STENCIL_OP_KEEP;
        ECOMPARISON_FUNCTION StencilFunc = COMPARISON_FUNC_ALWAYS;

        SStencilOpDesc() noexcept {}
        SStencilOpDesc(ESTENCIL_OP _StencilFailOp, ESTENCIL_OP _StencilDepthFailOp, ESTENCIL_OP _StencilPassOp,
                       ECOMPARISON_FUNCTION _StencilFunc) noexcept
            : StencilFailOp(_StencilFailOp)
            , StencilDepthFailOp(_StencilDepthFailOp)
            , StencilPassOp(_StencilPassOp)
            , StencilFunc(_StencilFunc)
        {}

        bool operator==(const SStencilOpDesc& rhs) const {
            return StencilFailOp == rhs.StencilFailOp
                && StencilDepthFailOp == rhs.StencilDepthFailOp
                && StencilPassOp == rhs.StencilPassOp
                && StencilFunc == rhs.StencilFunc;
        }
    };
}