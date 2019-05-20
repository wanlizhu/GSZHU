#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    struct GSZHU_API SBufferData {
        const void* Data = nullptr;
        uint32_t Size = 0;

        SBufferData() noexcept;
        SBufferData(const void* _Data, uint32_t _Size) noexcept;
    };
}