#pragma once

#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct GSZHU_API SDeviceObjectAttribs {
        const char* Name = nullptr;

        SDeviceObjectAttribs() noexcept;
        explicit SDeviceObjectAttribs(const char* _Name) noexcept;
    };
}