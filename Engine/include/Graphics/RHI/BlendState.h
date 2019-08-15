#pragma once

#include "Graphics/Config.h"
#include "Graphics/RHI/RenderTargetBlend.h"
#include <vector>

namespace ZHU
{
    // This structure describes the blend state and is part of the GraphicsPipelineDesc.
    struct ZHU_GRAPHICS_API BlendState
    {
        bool EnableAlphaToCoverage = false;
        bool EnableIndependentBlend = false;
		std::vector<RenderTargetBlend> RenderTargets;
    };
}