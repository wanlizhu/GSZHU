#pragma once

#include "Common/Config.h"

namespace NTGS {
    enum ColorFormat {
        DEPTH32F,
        STENCIL8,
        DEPTH24F_STENCIL8,

        R8,
        R32F,

        RGB8,
        RGB32F,
        RGBA8,
        RGBA32F,

        RGB8_SRGB, // Standard RGB (gamma = 2.2)
        RGB32F_SRGB, // Standard RGB (gamma = 2.2)
        RGBA8_SRGB, // Standard RGB (gamma = 2.2)
        RGBA32F_SRGB, // Standard RGB (gamma = 2.2)
    };
}