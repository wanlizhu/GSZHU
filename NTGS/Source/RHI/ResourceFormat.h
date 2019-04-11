#pragma once

#include "Common/Config.h"

namespace NTGS {
    enum ColorFormat {
        UNDEFINED,

        // depth formats
        DEPTH24_STENCIL8,
        DEPTH_FLOAT,
        
        // general formats
        RED_UINT8,
        RED_FLOAT,
        
        RG_UINT8,
        RG_FLOAT,
        
        RGB_UINT8,
        RGB_UINT8_SRGB, // Standard RGB (gamma = 2.2)
        RGB_FLOAT,
        RGB_FLOAT_SRGB, // Standard RGB (gamma = 2.2)

        RGBA_UINT8,
        RGBA_UINT8_SRGB, // Standard RGB (gamma = 2.2)
        RGBA_FLOAT,
        RGBA_FLOAT_SRGB, // Standard RGB (gamma = 2.2)

        // compressed formats
        X1_UNORM,
        X1_UNORM_SRGB,
        X5_UNORM,
        X5_SNORM,
    };
}