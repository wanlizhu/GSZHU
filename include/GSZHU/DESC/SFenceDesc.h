#pragma once

#include <GSZHU/SDeviceObjectAttribs.h>
#include <unordered_map>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct GSZHU_API SFenceDesc : public SDeviceObjectAttribs {

    };
}

namespace std {
    template<>
    struct hash<GSZHU::SFenceDesc> {
        size_t operator()(const GSZHU::SFenceDesc& Desc) const {
            return GSZHU::ComputeHash(0);
        }
    };
}