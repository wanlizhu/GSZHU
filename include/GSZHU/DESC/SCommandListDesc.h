#pragma once

#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct GSZHU_API SCommandListDesc : public SDeviceObjectAttribs {

    };
}

namespace std {
    template<>
    struct hash<GSZHU::SCommandListDesc> {
        size_t operator()(const GSZHU::SCommandListDesc& Desc) const {
            return GSZHU::ComputeHash(0);
        }
    };
}