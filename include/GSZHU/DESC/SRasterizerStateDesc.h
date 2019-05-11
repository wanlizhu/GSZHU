#pragma once

#include <GSZHU/ENUM/EFillMode.h>
#include <GSZHU/ENUM/ECullMode.h>

namespace GSZHU {
    // This structure describes the rasterizer state and is part of the GraphicsPipelineDesc.
    struct SRasterizerStateDesc {
        EFILL_MODE FillMode = FILL_MODE_SOLID;
        ECULL_MODE CullMode = CULL_MODE_BACK;
        bool FrontCounterClockwise = false;
        bool DepthClipEnable = true;
        bool ScissorEnable = false;
        bool AntialiasedLineEnable = false;
        INT DepthBias = 0;
        float DepthBiasClamp = 0.f;
        float SlopeScaledDepthBias = 0.f;

        SRasterizerStateDesc() noexcept {}
        SRasterizerStateDesc(EFILL_MODE _FillMode,
            ECULL_MODE _CullMode) noexcept 
            : FillMode(_FillMode)
            , CullMode(_CullMode)
        {}

        bool operator == (const SRasterizerStateDesc& rhs) const {
            return FillMode == rhs.FillMode             
                && CullMode == rhs.CullMode            
                && FrontCounterClockwise == rhs.FrontCounterClockwise
                && DepthBias == rhs.DepthBias        
                && DepthBiasClamp == rhs.DepthBiasClamp    
                && SlopeScaledDepthBias == rhs.SlopeScaledDepthBias
                && DepthClipEnable == rhs.DepthClipEnable 
                && ScissorEnable == rhs.ScissorEnable     
                && AntialiasedLineEnable == rhs.AntialiasedLineEnable;
        }
    };
}