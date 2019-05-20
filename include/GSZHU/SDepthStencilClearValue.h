#pragma once

#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct GSZHU_API SDepthStencilClearValue {
        float Depth = 1.f;
        uint8_t Stencil = 0;

        SDepthStencilClearValue() noexcept;
        SDepthStencilClearValue(float _Depth, uint8_t _Stencil) noexcept;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SDepthStencilClearValue> {
        size_t operator()(const GSZHU::SDepthStencilClearValue& Val) const {
            return GSZHU::ComputeHash(Val.Depth, Val.Stencil);
        }
    };
}