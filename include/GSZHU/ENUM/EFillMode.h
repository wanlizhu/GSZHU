#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum EFILL_MODE : uint8_t {
        FILL_MODE_UNDEFINED = 0,
        FILL_MODE_WIREFRAME,
        FILL_MODE_SOLID,

        FILL_MODE_NUM_MODES
    };
}