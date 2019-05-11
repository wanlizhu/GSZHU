#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    static constexpr UINT MAX_BUFFER_SLOTS = 32;
    static constexpr UINT MAX_RENDER_TARGETS = 8;
    static constexpr UINT MAX_VIEWPORTS = 16;
    static constexpr UINT MAX_SHADERS_IN_PIPELINE = 5;
    static constexpr UINT MAX_LAYOUT_ELEMENTS = 16;

    static constexpr UINT AUTO_OFFSET = static_cast<UINT>(-1);
    static constexpr UINT AUTO_STRIDE = static_cast<UINT>(-1);
}