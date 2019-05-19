#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // This enumeration describes which parts of the pipeline a resource can be bound to.
    // It is used by BufferDesc and TextureDesc
    enum EBIND_FLAGS : uint32_t {
        BIND_NONE = 0x0L, 
        BIND_VERTEX_BUFFER = 0x1L, // A buffer can be bound as a vertex buffer
        BIND_INDEX_BUFFER = 0x2L, // A buffer can be bound as an index buffer
        BIND_UNIFORM_BUFFER = 0x4L, // A buffer can be bound as a uniform buffer
                                        
        BIND_SHADER_RESOURCE = 0x8L, // This flag may not be combined with any other bind flag 
        BIND_STREAM_OUTPUT = 0x10L, // A buffer can be bound as a target for stream output stage
        BIND_RENDER_TARGET = 0x20L, // A texture can be bound as a render target
        BIND_DEPTH_STENCIL = 0x40L, // A texture can be bound as a depth-stencil target
        BIND_UNORDERED_ACCESS = 0x80L, // A buffer or a texture can be bound as an unordered access view
        BIND_INDIRECT_DRAW_ARGS = 0x100L // A buffer can be bound as the source buffer for indirect draw commands
    };

    DEFINE_FLAG_ENUM_OPS(EBIND_FLAGS)
}