#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum ECULL_MODE : INT8 {
        CULL_MODE_UNDEFINED = 0,
        CULL_MODE_NONE, // OpenGL counterpart: glDisable( GL_CULL_FACE )
        CULL_MODE_FRONT, // OpenGL counterpart: GL_FRONT.
        CULL_MODE_BACK,

        CULL_MODE_NUM_MODES
    };
}