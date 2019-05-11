#pragma once

#include <GSZHU/SDepthStencilClearValue.h>
#include <GSZHU/ENUM/ETextureFormat.h>

namespace GSZHU {
    struct SOptimizedClearValue {
        ETEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
        float Color[4] = { 0, 0, 0, 0 };
        SDepthStencilClearValue DepthStencil;

        bool operator == (const SOptimizedClearValue& rhs) const {
            return Format == rhs.Format 
                && Color[0] == rhs.Color[0] 
                && Color[1] == rhs.Color[1] 
                && Color[2] == rhs.Color[2] 
                && Color[3] == rhs.Color[3] 
                && DepthStencil.Depth == rhs.DepthStencil.Depth
                && DepthStencil.Stencil == rhs.DepthStencil.Stencil;
        }
    };
}