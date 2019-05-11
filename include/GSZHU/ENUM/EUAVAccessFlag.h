#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    /// Describes allowed unordered access view mode
    enum UAV_ACCESS_FLAG : INT {
        UAV_ACCESS_UNSPECIFIED = 0x00,
        UAV_ACCESS_FLAG_READ = 0x01,
        UAV_ACCESS_FLAG_WRITE = 0x02,
        UAV_ACCESS_FLAG_READ_WRITE = UAV_ACCESS_FLAG_READ | UAV_ACCESS_FLAG_WRITE
    };
}