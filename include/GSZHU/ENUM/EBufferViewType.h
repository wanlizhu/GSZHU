#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // This enumeration describes allowed view types for a buffer view. It is used by BufferViewDesc
    enum EBUFFER_VIEW_TYPE : UINT8 {
        BUFFER_VIEW_UNDEFINED = 0,
        BUFFER_VIEW_SHADER_RESOURCE,
        BUFFER_VIEW_UNORDERED_ACCESS,
        BUFFER_VIEW_NUM_VIEWS
    };
}