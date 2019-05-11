#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum EBLEND_OP : INT8 {
        BLEND_OP_UNDEFINED = 0,
        BLEND_OP_ADD,
        BLEND_OP_SUBTRACT,
        BLEND_OP_REV_SUBTRACT,
        BLEND_OP_MIN,
        BLEND_OP_MAX,
        BLEND_OP_NUM_OPERATIONS
    };
}