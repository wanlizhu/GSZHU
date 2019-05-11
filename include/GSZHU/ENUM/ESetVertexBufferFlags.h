#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // Defines allowed flags for IDeviceContext::SetVertexBuffers() function.
    enum ESET_VERTEX_BUFFERS_FLAGS : UINT8 {
        SET_VERTEX_BUFFERS_FLAG_NONE = 0x00,
        SET_VERTEX_BUFFERS_FLAG_RESET = 0x01
    };
    DEFINE_FLAG_ENUM_OPS(ESET_VERTEX_BUFFERS_FLAGS)
}