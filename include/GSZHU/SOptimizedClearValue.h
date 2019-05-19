#pragma once

#include <GSZHU/SDepthStencilClearValue.h>
#include <GSZHU/ENUM/ETextureFormat.h>
#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    struct SOptimizedClearValue {
        ETEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
        float Color[4] = { 0, 0, 0, 0 };
        SDepthStencilClearValue DepthStencil;

        bool operator==(const SOptimizedClearValue& rhs) const {
            return Format == rhs.Format 
                && FLT_EQ(Color[0] == rhs.Color[0]) 
                && FLT_EQ(Color[1] == rhs.Color[1]) 
                && FLT_EQ(Color[2] == rhs.Color[2]) 
                && FLT_EQ(Color[3] == rhs.Color[3]) 
                && DepthStencil.Depth == rhs.DepthStencil.Depth
                && DepthStencil.Stencil == rhs.DepthStencil.Stencil;
        }
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