#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum ECOLOR_MASK : INT8 {
        COLOR_MASK_RED = 1,
        COLOR_MASK_GREEN = 2,
        COLOR_MASK_BLUE = 4,
        COLOR_MASK_ALPHA = 8,
        COLOR_MASK_ALL = (((COLOR_MASK_RED | COLOR_MASK_GREEN) | COLOR_MASK_BLUE) | COLOR_MASK_ALPHA)
    };
}