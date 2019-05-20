#include <GSZHU/SOptimizedClearValue.h>

namespace GSZHU {
    bool SOptimizedClearValue::operator==(const SOptimizedClearValue& rhs) const {
        return Format == rhs.Format 
            && FLT_EQ(Color[0], rhs.Color[0]) 
            && FLT_EQ(Color[1], rhs.Color[1]) 
            && FLT_EQ(Color[2], rhs.Color[2]) 
            && FLT_EQ(Color[3], rhs.Color[3]) 
            && DepthStencil.Depth == rhs.DepthStencil.Depth
            && DepthStencil.Stencil == rhs.DepthStencil.Stencil;
    }
}