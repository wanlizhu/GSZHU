#pragma once

#include <NTGS/Graphics/GraphicsEnums.h>

namespace NTGS {
    enum STENCIL_OP : INT8 {
        STENCIL_OP_UNDEFINED = 0,
        STENCIL_OP_KEEP = 1,
        STENCIL_OP_ZERO = 2,
        STENCIL_OP_REPLACE = 3,
        STENCIL_OP_INCR_SAT = 4,
        STENCIL_OP_DECR_SAT = 5,
        STENCIL_OP_INVERT = 6,
        STENCIL_OP_INCR_WRAP = 7,
        STENCIL_OP_DECR_WRAP = 8,
        STENCIL_OP_NUM_OPS
    };

    struct StencilOpDesc {
        STENCIL_OP StencilFailOp = STENCIL_OP_KEEP;
        STENCIL_OP StencilDepthFailOp = STENCIL_OP_KEEP;
        STENCIL_OP StencilPassOp = STENCIL_OP_KEEP;
        COMPARISON_FUNCTION StencilFunc = COMPARISON_FUNC_ALWAYS;

        StencilOpDesc() = default;
        StencilOpDesc(STENCIL_OP _StencilFailOp,
                      STENCIL_OP _StencilDepthFailOp,
                      STENCIL_OP _StencilPassOp,
                      COMPARISON_FUNCTION _StencilFunc) noexcept 
            : StencilFailOp(_StencilFailOp)
            , StencilDepthFailOp(_StencilDepthFailOp)
            , StencilPassOp(_StencilPassOp)
            , StencilFunc(_StencilFunc) 
        {}

        bool operator == (const StencilOpDesc& RHS) const {
            return StencilFailOp == RHS.StencilFailOp &&
                StencilDepthFailOp == RHS.StencilDepthFailOp &&
                StencilPassOp == RHS.StencilPassOp &&
                StencilFunc == RHS.StencilFunc;
        }
    };

    struct DepthStencilStateDesc {
        bool DepthEnable = True;
        bool DepthWriteEnable = True;
        COMPARISON_FUNCTION DepthFunc = COMPARISON_FUNC_LESS;
        bool StencilEnable = false;
        UINT8 StencilReadMask = 0xFF;
        UINT8 StencilWriteMask = 0xFF;
        StencilOpDesc FrontFace;
        StencilOpDesc BackFace;

        DepthStencilStateDesc() = default;
        DepthStencilStateDesc(bool _DepthEnable,
                              bool _DepthWriteEnable,
                              COMPARISON_FUNCTION _DepthFunc = DepthStencilStateDesc{}.DepthFunc,
                              bool _StencilEnable = DepthStencilStateDesc{}.StencilEnable,
                              UINT8 _StencilReadMask = DepthStencilStateDesc{}.StencilReadMask,
                              UINT8 _StencilWriteMask = DepthStencilStateDesc{}.StencilWriteMask,
                              StencilOpDesc _FrontFace = StencilOpDesc{},
                              StencilOpDesc _BackFace = StencilOpDesc{}) noexcept 
            : DepthEnable(_DepthEnable)
            , DepthWriteEnable(_DepthWriteEnable)
            , DepthFunc(_DepthFunc)
            , StencilEnable(_StencilEnable)
            , StencilReadMask(_StencilReadMask)
            , StencilWriteMask(_StencilWriteMask)
            , FrontFace(_FrontFace)
            , BackFace(_BackFace) 
        {}

        bool operator == (const DepthStencilStateDesc& RHS) const {
            return  DepthEnable == RHS.DepthEnable &&
                DepthWriteEnable == RHS.DepthWriteEnable &&
                DepthFunc == RHS.DepthFunc &&
                StencilEnable == RHS.StencilEnable &&
                StencilReadMask == RHS.StencilReadMask &&
                StencilWriteMask == RHS.StencilWriteMask &&
                FrontFace == RHS.FrontFace &&
                BackFace == RHS.BackFace;
        }
    };
}