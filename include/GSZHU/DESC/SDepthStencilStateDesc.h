#pragma once

#include <GSZHU/DESC/SStencilOperationDesc.h>

namespace GSZHU {
    // This structure describes the depth stencil state and is part of the GraphicsPipelineDesc.
    struct SDepthStencilStateDesc {
        bool DepthEnable = true;
        bool DepthWriteEnable = true;
        ECOMPARISON_FUNCTION DepthFunc = COMPARISON_FUNC_LESS;

        bool StencilEnable = false;
        UINT8 StencilReadMask = 0xFF;
        UINT8 StencilWriteMask = 0xFF;

        SStencilOpDesc FrontFace;
        SStencilOpDesc BackFace;

        SDepthStencilStateDesc() noexcept {}
        SDepthStencilStateDesc(bool _DepthEnable, bool _DepthWriteEnable) noexcept
            : DepthEnable(_DepthEnable)
            , DepthWriteEnable(_DepthWriteEnable)
        {}

        bool operator==(const SDepthStencilStateDesc& rhs) const {
            return  DepthEnable == rhs.DepthEnable
                 && DepthWriteEnable == rhs.DepthWriteEnable
                 && DepthFunc == rhs.DepthFunc
                 && StencilEnable == rhs.StencilEnable
                 && StencilReadMask == rhs.StencilReadMask
                 && StencilWriteMask == rhs.StencilWriteMask
                 && FrontFace == rhs.FrontFace
                 && BackFace == rhs.BackFace;
        }
    };
}