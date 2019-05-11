#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // This enumeration is used by BufferDesc structure.
    enum EBUFFER_MODE : UINT8 {
        BUFFER_MODE_UNDEFINED = 0,
        BUFFER_MODE_FORMATTED, // Formated buffer. Access to the buffer will use format conversion operations.
        BUFFER_MODE_STRUCTURED, // In this mode, ElementByteStride member of BufferDesc defines the structure stride.
        BUFFER_MODE_RAW, // In this mode, the buffer is accessed as raw bytes. 

        BUFFER_MODE_NUM_MODES
    };
}