#pragma once

#include <GSZHU/DESC/SRenderTargetBlendDesc.h>
#include <GSZHU/Constants.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    // This structure describes the blend state and is part of the GraphicsPipelineDesc.
    struct SBlendStateDesc {
        bool AlphaToCoverageEnable = false; // Specifies whether to use alpha-to-coverage as a multisampling technique 
        bool IndependentBlendEnable = false; // Specifies whether to enable independent blending in simultaneous render targets. 
        SRenderTargetBlendDesc RenderTargetArray[MAX_RENDER_TARGETS];

        SBlendStateDesc() noexcept;
        SBlendStateDesc(bool _AlphaToCoverageEnable,
                        bool _IndependentBlendEnable,
                        const SRenderTargetBlendDesc& RT0 = SRenderTargetBlendDesc()) noexcept;

        bool operator==(const SBlendStateDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SBlendStateDesc> {
        size_t operator()(const GSZHU::SBlendStateDesc& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.AlphaToCoverageEnable),
                                      static_cast<int>(Desc.IndependentBlendEnable),
                                      GSZHU::ComputeArrayHash<GSZHU::SRenderTargetBlendDesc>(Desc.RenderTargetArray, GSZHU::MAX_RENDER_TARGETS));
        }
    };
}