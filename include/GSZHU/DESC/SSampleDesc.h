#pragma once

#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct SSampleDesc {
        uint8_t Count = 1;
        uint8_t Quality = 0;

        SSampleDesc() noexcept;
        SSampleDesc(uint8_t _Count, uint8_t _Quality) noexcept;

        bool operator==(const SSampleDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SSampleDesc> {
        size_t operator()(const GSZHU::SSampleDesc& Desc) const {
            return GSZHU::ComputeHash(Desc.Count, Desc.Quality);
        }
    };
}