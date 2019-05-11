#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum EMAP_FLAGS : UINT8 {
        MAP_FLAG_NONE = 0x000,
        MAP_FLAG_DO_NOT_WAIT = 0x001,
        MAP_FLAG_DISCARD = 0x002,
        MAP_FLAG_DO_NOT_SYNCHRONIZE = 0x004
    };
    DEFINE_FLAG_ENUM_OPS(EMAP_FLAGS)
}