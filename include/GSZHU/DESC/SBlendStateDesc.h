#pragma once

#include <GSZHU/DESC/SRenderTargetBlendDesc.h>
#include <GSZHU/Constants.h>

namespace GSZHU {
    // This structure describes the blend state and is part of the GraphicsPipelineDesc.
    struct SBlendStateDesc {
        bool AlphaToCoverageEnable = false; // Specifies whether to use alpha-to-coverage as a multisampling technique 
        bool IndependentBlendEnable = false; // Specifies whether to enable independent blending in simultaneous render targets. 
        SRenderTargetBlendDesc RenderTargets[MAX_RENDER_TARGETS];

        SBlendStateDesc() noexcept {}
        SBlendStateDesc(bool _AlphaToCoverageEnable, bool _IndependentBlendEnable) noexcept
            : AlphaToCoverageEnable(_AlphaToCoverageEnable)
            , IndependentBlendEnable(_IndependentBlendEnable)
        {}

        bool operator==(const SBlendStateDesc& rhs) const {
            bool isEqual = true;
            for (int i = 0; i < MAX_RENDER_TARGETS; ++i)
                if (!(RenderTargets[i] == rhs.RenderTargets[i])) {
                    isEqual = false;
                    break;
                }

            return  isEqual &&
                AlphaToCoverageEnable == rhs.AlphaToCoverageEnable
                && IndependentBlendEnable == rhs.IndependentBlendEnable;
        }
    };
}