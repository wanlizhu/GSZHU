#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum ESTATE_TRANSITION_TYPE : UINT8 {
        STATE_TRANSITION_TYPE_IMMEDIATE = 0,
        STATE_TRANSITION_TYPE_BEGIN,
        STATE_TRANSITION_TYPE_END
    };
}