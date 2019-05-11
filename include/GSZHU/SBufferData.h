#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    struct SBufferData {
        const void* Data = nullptr;
        UINT Size = 0;

        SBufferData() noexcept {}
    };
}