#include <GSZHU/DESC/SDepthStencilStateDesc.h>

namespace GSZHU {
    SDepthStencilStateDesc::SDepthStencilStateDesc() noexcept
    {}

    SDepthStencilStateDesc::SDepthStencilStateDesc(bool _DepthEnable,
                                                   bool _DepthWriteEnable,
                                                   ECOMPARISON_FUNCTION _DepthFunc,
                                                   bool _StencilEnable,
                                                   uint8_t _StencilReadMask,
                                                   uint8_t _StencilWriteMask,
                                                   SStencilOpDesc _FrontFace,
                                                   SStencilOpDesc _BackFace) noexcept
        : DepthEnable(_DepthEnable)
        , DepthWriteEnable(_DepthWriteEnable)
        , DepthFunc(_DepthFunc)
        , StencilEnable(_StencilEnable)
        , StencilReadMask(_StencilReadMask)
        , StencilWriteMask(_StencilWriteMask)
        , FrontFace(_FrontFace)
        , BackFace(_BackFace)
    {}

    bool SDepthStencilStateDesc::operator==(const SDepthStencilStateDesc& rhs) const {
        return DepthEnable == rhs.DepthEnable
            && DepthWriteEnable == rhs.DepthWriteEnable
            && DepthFunc == rhs.DepthFunc
            && StencilEnable == rhs.StencilEnable
            && StencilReadMask == rhs.StencilReadMask
            && StencilWriteMask == rhs.StencilWriteMask
            && FrontFace == rhs.FrontFace
            && BackFace == rhs.BackFace;
    }
}