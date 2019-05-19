#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    struct SDepthStencilClearValue {
        float Depth = 1.f;
        uint8_t Stencil = 0;

        SDepthStencilClearValue() noexcept {}
        SDepthStencilClearValue(float _Depth, uint8_t _Stencil) noexcept 
            : Depth(_Depth)
            , Stencil(_Stencil)
        {}
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