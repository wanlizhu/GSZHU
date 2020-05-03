#pragma once

#include "utils/preprocess.h"

namespace djv
{
    enum class Format
    {
        Undefined = 0,

        D32_FLOAT,
        S8_UINT,
        D24_FLOAT_S8_UINT,

        RGBA8_UNORM_SRGB,
        RGBA32_FLOAT,
    };
    ENUM_CLASS_OP(Format)

}