#pragma once

#include <NTGS/Core/Config.h>

namespace NTGS {
    enum BLEND_FACTOR : INT8 {
        BLEND_FACTOR_UNDEFINED = 0,
        BLEND_FACTOR_ZERO,
        BLEND_FACTOR_ONE,
        BLEND_FACTOR_SRC_COLOR,
        BLEND_FACTOR_INV_SRC_COLOR,
        BLEND_FACTOR_SRC_ALPHA, 
        BLEND_FACTOR_INV_SRC_ALPHA,
        BLEND_FACTOR_DEST_ALPHA,
        BLEND_FACTOR_INV_DEST_ALPHA,
        BLEND_FACTOR_DEST_COLOR,
        BLEND_FACTOR_INV_DEST_COLOR,
        BLEND_FACTOR_SRC_ALPHA_SAT,
        BLEND_FACTOR_BLEND_FACTOR,
        BLEND_FACTOR_INV_BLEND_FACTOR,
        BLEND_FACTOR_SRC1_COLOR,
        BLEND_FACTOR_INV_SRC1_COLOR,
        BLEND_FACTOR_SRC1_ALPHA,
        BLEND_FACTOR_INV_SRC1_ALPHA,
        BLEND_FACTOR_NUM_FACTORS
    };

    enum BLEND_OPERATION : INT8 {
        BLEND_OPERATION_UNDEFINED = 0,
        BLEND_OPERATION_ADD,
        BLEND_OPERATION_SUBTRACT,
        BLEND_OPERATION_REV_SUBTRACT,
        BLEND_OPERATION_MIN,
        BLEND_OPERATION_MAX,
        BLEND_OPERATION_NUM_OPERATIONS
    };

    enum COLOR_MASK : INT8 {
        COLOR_MASK_RED = 1,
        COLOR_MASK_GREEN = 2,
        COLOR_MASK_BLUE = 4,
        COLOR_MASK_ALPHA = 8,
        COLOR_MASK_ALL = (((COLOR_MASK_RED | COLOR_MASK_GREEN) | COLOR_MASK_BLUE) | COLOR_MASK_ALPHA)
    };

    enum LOGIC_OPERATION : INT8 {
        LOGIC_OP_CLEAR = 0,
        LOGIC_OP_SET,
        LOGIC_OP_COPY,
        LOGIC_OP_COPY_INVERTED,
        LOGIC_OP_NOOP,
        LOGIC_OP_INVERT,
        LOGIC_OP_AND,
        LOGIC_OP_NAND,
        LOGIC_OP_OR,
        LOGIC_OP_NOR,
        LOGIC_OP_XOR,
        LOGIC_OP_EQUIV,
        LOGIC_OP_AND_REVERSE,
        LOGIC_OP_AND_INVERTED,
        LOGIC_OP_OR_REVERSE,
        LOGIC_OP_OR_INVERTED,
        LOGIC_OP_NUM_OPERATIONS
    };

    struct RenderTargetBlendDesc {
        bool BlendEnable = false;
        bool LogicOperationEnable = false;
        BLEND_FACTOR SrcBlend = BLEND_FACTOR_ONE;
        BLEND_FACTOR DestBlend = BLEND_FACTOR_ZERO;
        BLEND_OPERATION BlendOp = BLEND_OPERATION_ADD;
        BLEND_FACTOR    SrcBlendAlpha = BLEND_FACTOR_ONE;
        BLEND_FACTOR    DestBlendAlpha = BLEND_FACTOR_ZERO;
        BLEND_OPERATION BlendOpAlpha = BLEND_OPERATION_ADD;
        LOGIC_OPERATION LogicOp = LOGIC_OP_NOOP;
        UINT8 RenderTargetWriteMask = COLOR_MASK_ALL;

        RenderTargetBlendDesc() = default;
        explicit  RenderTargetBlendDesc(bool _BlendEnable,
                                        bool _LogicOperationEnable = RenderTargetBlendDesc{}.LogicOperationEnable,
                                        BLEND_FACTOR _SrcBlend = RenderTargetBlendDesc{}.SrcBlend,
                                        BLEND_FACTOR  _DestBlend = RenderTargetBlendDesc{}.DestBlend,
                                        BLEND_OPERATION _BlendOp = RenderTargetBlendDesc{}.BlendOp,
                                        BLEND_FACTOR _SrcBlendAlpha = RenderTargetBlendDesc{}.SrcBlendAlpha,
                                        BLEND_FACTOR _DestBlendAlpha = RenderTargetBlendDesc{}.DestBlendAlpha,
                                        BLEND_OPERATION _BlendOpAlpha = RenderTargetBlendDesc{}.BlendOpAlpha,
                                        LOGIC_OPERATION _LogicOp = RenderTargetBlendDesc{}.LogicOp,
                                        UINT8 _RenderTargetWriteMask = RenderTargetBlendDesc{}.RenderTargetWriteMask) 
            : BlendEnable(_BlendEnable),
              LogicOperationEnable(_LogicOperationEnable),
              SrcBlend(_SrcBlend),
              DestBlend(_DestBlend),
              BlendOp(_BlendOp),
              SrcBlendAlpha(_SrcBlendAlpha),
              DestBlendAlpha(_DestBlendAlpha),
              BlendOpAlpha(_BlendOpAlpha),
              LogicOp(_LogicOp),
              RenderTargetWriteMask(_RenderTargetWriteMask) {}

        bool operator == (const RenderTargetBlendDesc& RHS) const {
            return BlendEnable == RHS.BlendEnable &&
                LogicOperationEnable == RHS.LogicOperationEnable &&
                SrcBlend == RHS.SrcBlend &&
                DestBlend == RHS.DestBlend &&
                BlendOp == RHS.BlendOp &&
                SrcBlendAlpha == RHS.SrcBlendAlpha &&
                DestBlendAlpha == RHS.DestBlendAlpha &&
                BlendOpAlpha == RHS.BlendOpAlpha &&
                LogicOp == RHS.LogicOp &&
                RenderTargetWriteMask == RHS.RenderTargetWriteMask;
        }
    };

    struct BlendStateDesc {
        bool AlphaToCoverageEnable = false;
        bool IndependentBlendEnable = false;
        static constexpr int MaxRenderTargets = 8;
        RenderTargetBlendDesc RenderTargets[MaxRenderTargets];

        BlendStateDesc() = default;

        BlendStateDesc(bool _AlphaToCoverageEnable,
                       bool _IndependentBlendEnable,
                       const RenderTargetBlendDesc& RT0 = RenderTargetBlendDesc{}) noexcept 
            : AlphaToCoverageEnable(_AlphaToCoverageEnable),
              IndependentBlendEnable(_IndependentBlendEnable),
              RenderTargets{ RT0 } {}

        bool operator == (const BlendStateDesc& RHS) const {
            bool bRTsEqual = true;
            for (int i = 0; i < MaxRenderTargets; ++i)
                if (!(RenderTargets[i] == RHS.RenderTargets[i])) {
                    bRTsEqual = false;
                    break;
                }

            return  bRTsEqual &&
                AlphaToCoverageEnable == RHS.AlphaToCoverageEnable &&
                IndependentBlendEnable == RHS.IndependentBlendEnable;
        }
    };
}