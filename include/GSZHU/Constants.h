#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    static constexpr uint32_t MAX_BUFFER_SLOTS = 32;
    static constexpr uint32_t MAX_RENDER_TARGETS = 8;
    static constexpr uint32_t MAX_VIEWPORTS = 16;
    static constexpr uint32_t MAX_SHADERS_IN_PIPELINE = 5;
    static constexpr uint32_t MAX_LAYOUT_ELEMENTS = 16;

    static constexpr uint32_t AUTO_OFFSET = static_cast<uint32_t>(-1);
    static constexpr uint32_t AUTO_STRIDE = static_cast<uint32_t>(-1);
}