#pragma once

#include <GSZHU/BasicTools.h>
#include <GSZHU/ENUM/EComparisonFunction.h>
#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/ENUM/EFilterType.h>
#include <GSZHU/ENUM/ETextureAddressMode.h>

namespace GSZHU {
    // This structure describes the sampler state which is used in a call to IRenderDevice::CreateSampler() to create a sampler object.
    struct GSZHU_API SSamplerDesc : public SDeviceObjectAttribs {
        FILTER_TYPE MinFilter = FILTER_TYPE_LINEAR;
        FILTER_TYPE MagFilter = FILTER_TYPE_LINEAR;
        FILTER_TYPE MipFilter = FILTER_TYPE_LINEAR;
        ETEXTURE_ADDRESS_MODE AddressU = TEXTURE_ADDRESS_CLAMP;
        ETEXTURE_ADDRESS_MODE AddressV = TEXTURE_ADDRESS_CLAMP;
        ETEXTURE_ADDRESS_MODE AddressW = TEXTURE_ADDRESS_CLAMP;

        float MipLODBias = 0;
        uint32_t MaxAnisotropy = 0;
        ECOMPARISON_FUNCTION ComparisonFunc = COMPARISON_FUNC_NEVER;
        float BorderColor[4] = { 0, 0, 0, 0 };
        float MinLOD = 0;
        float MaxLOD = +3.402823466e+38F;

        SSamplerDesc() noexcept;
        SSamplerDesc(FILTER_TYPE _MinFilter, FILTER_TYPE _MagFilter, FILTER_TYPE _MipFilter) noexcept;
        
        bool operator==(const SSamplerDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SSamplerDesc> {
        size_t operator()(const GSZHU::SSamplerDesc& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.MinFilter),
                                      static_cast<int>(Desc.MagFilter),
                                      static_cast<int>(Desc.MipFilter),
                                      static_cast<int>(Desc.AddressU),
                                      static_cast<int>(Desc.AddressV),
                                      static_cast<int>(Desc.AddressW),
                                      Desc.MipLODBias,
                                      Desc.MaxAnisotropy,
                                      static_cast<int>(Desc.ComparisonFunc),
                                      GSZHU::ComputeArrayHash<float>(Desc.BorderColor, 4),
                                      Desc.MinLOD,
                                      Desc.MaxLOD);
        }
    };
}