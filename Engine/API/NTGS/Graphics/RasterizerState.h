#pragma once

#include <NTGS/Graphics/GraphicsEnums.h>

namespace NTGS {
    enum FILL_MODE : INT8 {
        FILL_MODE_UNDEFINED = 0,
        FILL_MODE_WIREFRAME,
        FILL_MODE_SOLID,
        FILL_MODE_NUM_MODES
    };

    enum CULL_MODE : INT8 {
        CULL_MODE_UNDEFINED = 0,
        CULL_MODE_NONE,
        CULL_MODE_FRONT,
        CULL_MODE_BACK,
        CULL_MODE_NUM_MODES
    };

    struct RasterizerStateDesc {
        FILL_MODE FillMode = FILL_MODE_SOLID;
        CULL_MODE CullMode = CULL_MODE_BACK;
        bool FrontCounterClockwise = False;
        bool DepthClipEnable = True;
        bool ScissorEnable = False;
        bool AntialiasedLineEnable = False;
        INT DepthBias = 0;
        FLOAT DepthBiasClamp = 0.f;
        FLOAT SlopeScaledDepthBias = 0.f;

        RasterizerStateDesc() = default;
        RasterizerStateDesc(FILL_MODE _FillMode,
                            CULL_MODE _CullMode,
                            bool _FrontCounterClockwise = RasterizerStateDesc{}.FrontCounterClockwise,
                            bool _DepthClipEnable = RasterizerStateDesc{}.DepthClipEnable,
                            bool _ScissorEnable = RasterizerStateDesc{}.ScissorEnable,
                            bool _AntialiasedLineEnable = RasterizerStateDesc{}.AntialiasedLineEnable,
                            INT _DepthBias = RasterizerStateDesc{}.DepthBias,
                            FLOAT _DepthBiasClamp = RasterizerStateDesc{}.DepthBiasClamp,
                            FLOAT _SlopeScaledDepthBias = RasterizerStateDesc{}.SlopeScaledDepthBias) noexcept 
            : FillMode(_FillMode),
              CullMode(_CullMode),
              FrontCounterClockwise(_FrontCounterClockwise),
              DepthClipEnable(_DepthClipEnable),
              ScissorEnable(_ScissorEnable),
              AntialiasedLineEnable(_AntialiasedLineEnable),
              DepthBias(_DepthBias),
              DepthBiasClamp(_DepthBiasClamp),
              SlopeScaledDepthBias(_SlopeScaledDepthBias) {}

        bool operator == (const RasterizerStateDesc& RHS) const {
            return  FillMode == RHS.FillMode &&
                CullMode == RHS.CullMode &&
                FrontCounterClockwise == RHS.FrontCounterClockwise &&
                DepthBias == RHS.DepthBias &&
                DepthBiasClamp == RHS.DepthBiasClamp &&
                SlopeScaledDepthBias == RHS.SlopeScaledDepthBias &&
                DepthClipEnable == RHS.DepthClipEnable &&
                ScissorEnable == RHS.ScissorEnable &&
                AntialiasedLineEnable == RHS.AntialiasedLineEnable;
        }
    };
}