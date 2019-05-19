#pragma once

#include <GSZHU/DESC/SStencilOperationDesc.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    // This structure describes the depth stencil state and is part of the GraphicsPipelineDesc.
    struct SDepthStencilStateDesc {
        bool DepthEnable = true;
        bool DepthWriteEnable = true;
        ECOMPARISON_FUNCTION DepthFunc = COMPARISON_FUNC_LESS;

        bool StencilEnable = false;
        uint8_t StencilReadMask = 0xFF;
        uint8_t StencilWriteMask = 0xFF;

        SStencilOpDesc FrontFace;
        SStencilOpDesc BackFace;

        SDepthStencilStateDesc() noexcept;
        SDepthStencilStateDesc(bool _DepthEnable, bool _DepthWriteEnable,
                               ECOMPARISON_FUNCTION _DepthFunc = SDepthStencilStateDesc{}.DepthFunc,
                               bool _StencilEnable = SDepthStencilStateDesc{}.StencilEnable,
                               uint8_t _StencilReadMask = SDepthStencilStateDesc{}.StencilReadMask,
                               uint8_t _StencilWriteMask = SDepthStencilStateDesc{}.StencilWriteMask,
                               SStencilOpDesc _FrontFace = SDepthStencilStateDesc{}.FrontFace,
                               SStencilOpDesc _BackFace = SDepthStencilStateDesc{}.BackFace) noexcept;

        bool operator==(const SDepthStencilStateDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SDepthStencilStateDesc> {
        size_t operator()(const GSZHU::SDepthStencilStateDesc& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.DepthEnable),
                                      static_cast<int>(Desc.DepthEnable),
                                      static_cast<int>(Desc.DepthFunc),
                                      static_cast<int>(Desc.StencilEnable),
                                      Desc.StencilReadMask,
                                      Desc.StencilWriteMask,
                                      std::hash<GSZHU::SStencilOpDesc>()(Desc.FrontFace),
                                      std::hash<GSZHU::SStencilOpDesc>()(Desc.BackFace));
        }
    };
}