#include <GSZHU/DESC/SStencilOperationDesc.h>

namespace GSZHU {
    SStencilOpDesc::SStencilOpDesc() noexcept
    {}

    SStencilOpDesc::SStencilOpDesc(ESTENCIL_OP _StencilFailOp,
                                   ESTENCIL_OP _StencilDepthFailOp, 
                                   ESTENCIL_OP _StencilPassOp,
                                   ECOMPARISON_FUNCTION _StencilFunc) noexcept
        : StencilFailOp(_StencilFailOp)
        , StencilDepthFailOp(_StencilDepthFailOp)
        , StencilPassOp(_StencilPassOp)
        , StencilFunc(_StencilFunc)
    {}

    bool SStencilOpDesc::operator==(const SStencilOpDesc& rhs) const {
        return StencilFailOp == rhs.StencilFailOp
            && StencilDepthFailOp == rhs.StencilDepthFailOp
            && StencilPassOp == rhs.StencilPassOp
            && StencilFunc == rhs.StencilFunc;
    }
}