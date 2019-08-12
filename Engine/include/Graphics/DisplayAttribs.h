#pragma once

#include "Graphics/Constants.h"

namespace ZHU
{
    struct DisplayAttribs
    {
        uint32_t Width = 0;
        uint32_t Height = 0;
        EPixelFormat Format = EPixelFormat::Unknown;
        uint32_t RefreshRateNumerator = 0;
        uint32_t RefreshRateDenominator = 0;
        EDisplayScaling Scaling = EDisplayScaling::Undefined;
        EDisplayOrder ScanOrder = EDisplayOrder::Undefined;
    };
}