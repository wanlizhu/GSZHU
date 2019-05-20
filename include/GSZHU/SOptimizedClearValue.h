#pragma once

#include <GSZHU/SDepthStencilClearValue.h>
#include <GSZHU/ENUM/ETextureFormat.h>
#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    struct GSZHU_API SOptimizedClearValue {
        ETEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
        float Color[4] = { 0, 0, 0, 0 };
        SDepthStencilClearValue DepthStencil;

        bool operator==(const SOptimizedClearValue& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SOptimizedClearValue> {
        size_t operator()(const GSZHU::SOptimizedClearValue& Val) const {
            return GSZHU::ComputeHash(static_cast<int>(Val.Format), 
                                      GSZHU::ComputeArrayHash<float>(Val.Color, 4),
                                      std::hash<GSZHU::SDepthStencilClearValue>()(Val.DepthStencil));
        }
    };
}