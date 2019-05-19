#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    struct SBufferData {
        const void* Data = nullptr;
        uint32_t Size = 0;

        SBufferData() noexcept {}
    };
}