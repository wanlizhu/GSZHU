#pragma once

#include "Core/Common.h"
#include "Graphics/RHI/RHIFlags.h"
#include "Graphics/RHI/Constants.h"

namespace Wanlix
{
    struct RasterizerStateDesc
    {
        EFillMode fillMode = EFillMode::Solid;
        ECullMode cullMode = ECullMode::Back;
        Bool frontCCW = true;
        Bool depthClipEnabled = true;
        Bool scissorEnabled = false;
        Bool lineAAEnabled = false;
        int depthBias = 0;
        float depthBiasClamp = 0.f;
        float slopeScaledDepthBias = 0.f;
    };

    inline constexpr bool operator==(const RasterizerStateDesc& lhs, const RasterizerStateDesc& rhs)
    {
        return lhs.fillMode == rhs.fillMode
            && lhs.cullMode == rhs.cullMode
            && lhs.frontCCW == rhs.frontCCW
            && lhs.depthClipEnabled == rhs.depthClipEnabled
            && lhs.scissorEnabled == rhs.scissorEnabled
            && lhs.lineAAEnabled == rhs.lineAAEnabled
            && lhs.depthBias == rhs.depthBias
            && Equals(lhs.depthBiasClamp, rhs.depthBiasClamp)
            && Equals(lhs.slopeScaledDepthBias, rhs.slopeScaledDepthBias);
    }
}