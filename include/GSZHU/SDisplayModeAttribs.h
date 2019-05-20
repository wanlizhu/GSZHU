#pragma once

#include <GSZHU/BasicTypes.h>
#include <GSZHU/ENUM/ETextureFormat.h>

namespace GSZHU {
    struct GSZHU_API SDisplayModeAttribs {
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

        uint32_t Width = 0;
        uint32_t Height = 0;
        ETEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
        uint32_t RefreshRateNumerator = 0;
        uint32_t RefreshRateDenominator = 0;
        SCALING Scaling = SCALING_UNSPECIFIED;
        SCANLINE_ORDER ScanlineOrder = SCANLINE_ORDER_UNSPECIFIED;
    };
}