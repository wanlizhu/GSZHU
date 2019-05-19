#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum ECLEAR_DEPTH_STENCIL_FLAGS : uint32_t {
        CLEAR_DEPTH_FLAG_NONE = 0x00,
        CLEAR_DEPTH_FLAG = 0x01,
        CLEAR_STENCIL_FLAG = 0x02
    };
    DEFINE_FLAG_ENUM_OPS(ECLEAR_DEPTH_STENCIL_FLAGS)
}