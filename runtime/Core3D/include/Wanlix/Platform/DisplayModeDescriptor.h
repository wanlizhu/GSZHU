#pragma once

#include "Wanlix/Core/Types.h"

namespace Wanlix
{
    struct DisplayModeDescriptor
    {
        Extent resolution;
        uint32_t refreshRate = 0; // in Hz

        bool operator<(const DisplayModeDescriptor& mode) const;
        bool operator==(const DisplayModeDescriptor& mode) const;
        bool operator!=(const DisplayModeDescriptor& mode) const;
        Extent GetExtentRatio() const;
    };
}