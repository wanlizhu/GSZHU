#pragma once

#include <GSZHU/ENUM/EFillMode.h>
#include <GSZHU/ENUM/ECullMode.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    // This structure describes the rasterizer state and is part of the GraphicsPipelineDesc.
    struct GSZHU_API SRasterizerStateDesc {
        EFILL_MODE FillMode = FILL_MODE_SOLID;
        ECULL_MODE CullMode = CULL_MODE_BACK;
        bool FrontCounterClockwise = false;
        bool DepthClipEnable = true;
        bool ScissorEnable = false;
        bool AntialiasedLineEnable = false;
        int DepthBias = 0;
        float DepthBiasClamp = 0.f;
        float SlopeScaledDepthBias = 0.f;

        SRasterizerStateDesc() noexcept;
        SRasterizerStateDesc(EFILL_MODE _FillMode, ECULL_MODE _CullMode) noexcept;

        bool operator==(const SRasterizerStateDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SRasterizerStateDesc> {
        size_t operator()(const GSZHU::SRasterizerStateDesc& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.FillMode),
                                      static_cast<int>(Desc.CullMode),
                                      static_cast<int>(Desc.FrontCounterClockwise),
                                      static_cast<int>(Desc.DepthClipEnable),
                                      static_cast<int>(Desc.ScissorEnable),
                                      static_cast<int>(Desc.AntialiasedLineEnable),
                                      Desc.DepthBias,
                                      Desc.DepthBiasClamp,
                                      Desc.SlopeScaledDepthBias);
        }
    };
}