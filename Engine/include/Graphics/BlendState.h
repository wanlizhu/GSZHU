#pragma once

#include "Graphics/RenderTargetBlend.h"

namespace ZHU
{
    // This structure describes the blend state and is part of the GraphicsPipelineDesc.
    struct BlendState
    {
        static constexpr int MaxRenderTargets = 8;

        bool EnableAlphaToCoverage = false;
        bool EnableIndependentBlend = false;
        RenderTargetBlend RenderTargets[MaxRenderTargets];
    };
}