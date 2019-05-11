#pragma once

namespace GSZHU {
    struct SDeviceObjectAttribs {
        const char* Name = nullptr;

        SDeviceObjectAttribs() noexcept {}
        explicit SDeviceObjectAttribs(const char* _Name) : Name(_Name) {}
    };
}