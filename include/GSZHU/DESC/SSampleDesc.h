#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    struct SSampleDesc {
        UINT8 Count = 1;
        UINT8 Quality = 0;

        SSampleDesc() noexcept {}
        SSampleDesc(UINT8 _Count, UINT8 _Quality)
            : Count(_Count)
            , Quality(_Quality)
        {}
    };
}