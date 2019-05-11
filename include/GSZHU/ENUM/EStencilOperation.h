#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum ESTENCIL_OP : INT8 {
        STENCIL_OP_UNDEFINED = 0,
        STENCIL_OP_KEEP = 1,
        STENCIL_OP_ZERO = 2,
        STENCIL_OP_REPLACE = 3,
        STENCIL_OP_INCR_SAT = 4,
        STENCIL_OP_DECR_SAT = 5,
        STENCIL_OP_INVERT = 6,
        STENCIL_OP_INCR_WRAP = 7,
        STENCIL_OP_DECR_WRAP = 8,

        STENCIL_OP_NUM_OPS
    };
}