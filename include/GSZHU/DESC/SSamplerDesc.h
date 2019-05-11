#pragma once

#include <GSZHU/BasicTypes.h>
#include <GSZHU/ENUM/EComparisonFunction.h>
#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/ENUM/EFilterType.h>
#include <GSZHU/ENUM/ETextureAddressMode.h>

namespace GSZHU {
    // This structure describes the sampler state which is used in a call to IRenderDevice::CreateSampler() to create a sampler object.
    struct SSamplerDesc : public SDeviceObjectAttribs {
        FILTER_TYPE MinFilter = FILTER_TYPE_LINEAR;
        FILTER_TYPE MagFilter = FILTER_TYPE_LINEAR;
        FILTER_TYPE MipFilter = FILTER_TYPE_LINEAR;
        ETEXTURE_ADDRESS_MODE AddressU = TEXTURE_ADDRESS_CLAMP;
        ETEXTURE_ADDRESS_MODE AddressV = TEXTURE_ADDRESS_CLAMP;
        ETEXTURE_ADDRESS_MODE AddressW = TEXTURE_ADDRESS_CLAMP;

        float MipLODBias = 0;
        UINT MaxAnisotropy = 0;
        ECOMPARISON_FUNCTION ComparisonFunc = COMPARISON_FUNC_NEVER;
        float BorderColor[4] = { 0, 0, 0, 0 };
        float MinLOD = 0;
        float MaxLOD = +3.402823466e+38F;

        SSamplerDesc() noexcept {}
        SSamplerDesc(FILTER_TYPE _MinFilter, FILTER_TYPE _MagFilter, FILTER_TYPE _MipFilter)
            : MinFilter(_MinFilter)
            , MagFilter(_MagFilter)
            , MipFilter(_MipFilter) {
            BorderColor[0] = BorderColor[1] = BorderColor[2] = BorderColor[3] = 0;
        }
        
        bool operator == (const SSamplerDesc& rhs) const {
            return MinFilter == rhs.MinFilter     
                && MagFilter == rhs.MagFilter     
                && MipFilter == rhs.MipFilter    
                && AddressU == rhs.AddressU    
                && AddressV == rhs.AddressV    
                && AddressW == rhs.AddressW    
                && MipLODBias == rhs.MipLODBias   
                && MaxAnisotropy == rhs.MaxAnisotropy 
                && ComparisonFunc == rhs.ComparisonFunc 
                && BorderColor[0] == rhs.BorderColor[0] 
                && BorderColor[1] == rhs.BorderColor[1] 
                && BorderColor[2] == rhs.BorderColor[2] 
                && BorderColor[3] == rhs.BorderColor[3] 
                && MinLOD == rhs.MinLOD     
                && MaxLOD == rhs.MaxLOD;
        }
    };
}