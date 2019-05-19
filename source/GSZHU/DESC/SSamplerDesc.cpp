#include <GSZHU/DESC/SSamplerDesc.h>
#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    SSamplerDesc::SSamplerDesc() noexcept
    {}

    SSamplerDesc::SSamplerDesc(FILTER_TYPE _MinFilter, FILTER_TYPE _MagFilter, FILTER_TYPE _MipFilter) noexcept
        : MinFilter(_MinFilter)
        , MagFilter(_MagFilter)
        , MipFilter(_MipFilter) {
        BorderColor[0] = BorderColor[1] = BorderColor[2] = BorderColor[3] = 0;
    }
        
    bool SSamplerDesc::operator==(const SSamplerDesc& rhs) const {
        return MinFilter == rhs.MinFilter     
            && MagFilter == rhs.MagFilter     
            && MipFilter == rhs.MipFilter    
            && AddressU == rhs.AddressU    
            && AddressV == rhs.AddressV    
            && AddressW == rhs.AddressW    
            && FLT_EQ(MipLODBias, rhs.MipLODBias)   
            && MaxAnisotropy == rhs.MaxAnisotropy 
            && ComparisonFunc == rhs.ComparisonFunc 
            && FLT_EQ(BorderColor[0], rhs.BorderColor[0]) 
            && FLT_EQ(BorderColor[1], rhs.BorderColor[1]) 
            && FLT_EQ(BorderColor[2], rhs.BorderColor[2]) 
            && FLT_EQ(BorderColor[3], rhs.BorderColor[3]) 
            && FLT_EQ(MinLOD, rhs.MinLOD)     
            && FLT_EQ(MaxLOD, rhs.MaxLOD);
    }
}