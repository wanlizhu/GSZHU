#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum EDRAW_FLAGS : uint8_t {
        DRAW_FLAG_NONE = 0x00,
        // Verify the sate of index and vertex buffers (if any) used by the draw command. 
        DRAW_FLAG_VERIFY_STATES = 0x01
    };
    DEFINE_FLAG_ENUM_OPS(EDRAW_FLAGS)
}