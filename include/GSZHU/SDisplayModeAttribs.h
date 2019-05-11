#pragma once

#include <GSZHU/BasicTypes.h>
#include <GSZHU/ENUM/ETextureFormat.h>

namespace GSZHU {
    struct SDisplayModeAttribs {
        enum SCALING {
            SCALING_UNSPECIFIED = 0,
            SCALING_CENTERED = 1,
            SCALING_STRETCHED = 2
        };
        enum SCANLINE_ORDER {
            SCANLINE_ORDER_UNSPECIFIED = 0,
            SCANLINE_ORDER_PROGRESSIVE = 1,
            SCANLINE_ORDER_UPPER_FIELD_FIRST = 2,
            SCANLINE_ORDER_LOWER_FIELD_FIRST = 3
        };

        UINT Width = 0;
        UINT Height = 0;
        ETEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
        UINT RefreshRateNumerator = 0;
        UINT RefreshRateDenominator = 0;
        SCALING Scaling = SCALING_UNSPECIFIED;
        SCANLINE_ORDER ScanlineOrder = SCANLINE_ORDER_UNSPECIFIED;
    };
}