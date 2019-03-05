#pragma once

#include <NTGS/Graphics/IDeviceObject.h>

namespace NTGS {
    struct SamplerDesc : public IDeviceObjectAttribs {
        FILTER_TYPE MinFilter = FILTER_TYPE_LINEAR;
        FILTER_TYPE MagFilter = FILTER_TYPE_LINEAR;
        FILTER_TYPE MipFilter = FILTER_TYPE_LINEAR;
        TEXTURE_ADDRESS_MODE AddressU = TEXTURE_ADDRESS_CLAMP;
        TEXTURE_ADDRESS_MODE AddressV = TEXTURE_ADDRESS_CLAMP;
        TEXTURE_ADDRESS_MODE AddressW = TEXTURE_ADDRESS_CLAMP;
        FLOAT MipLODBias = 0;
        UINT MaxAnisotropy = 0;
        COMPARISON_FUNCTION ComparisonFunc = COMPARISON_FUNC_NEVER;
        FLOAT BorderColor[4] = { 0, 0, 0, 0 };
        FLOAT MinLOD = 0;
        FLOAT MaxLOD = +3.402823466e+38F;

        SamplerDesc() = default;
        SamplerDesc(FILTER_TYPE _MinFilter,
                    FILTER_TYPE _MagFilter,
                    FILTER_TYPE _MipFilter,
                    TEXTURE_ADDRESS_MODE _AddressU = SamplerDesc{}.AddressU,
                    TEXTURE_ADDRESS_MODE _AddressV = SamplerDesc{}.AddressV,
                    TEXTURE_ADDRESS_MODE _AddressW = SamplerDesc{}.AddressW,
                    FLOAT _MipLODBias = SamplerDesc{}.MipLODBias,
                    UINT _MaxAnisotropy = SamplerDesc{}.MaxAnisotropy,
                    COMPARISON_FUNCTION _ComparisonFunc = SamplerDesc{}.ComparisonFunc,
                    FLOAT _MinLOD = SamplerDesc{}.MinLOD,
                    FLOAT _MaxLOD = SamplerDesc{}.MaxLOD) 
            : MinFilter(_MinFilter),
              MagFilter(_MagFilter),
              MipFilter(_MipFilter),
              AddressU(_AddressU),
              AddressV(_AddressV),
              AddressW(_AddressW),
              MipLODBias(_MipLODBias),
              MaxAnisotropy(_MaxAnisotropy),
              ComparisonFunc(_ComparisonFunc),
              MinLOD(_MinLOD),
              MaxLOD(_MaxLOD) {
            BorderColor[0] = BorderColor[1] = BorderColor[2] = BorderColor[3] = 0;
        }

        bool operator == (const SamplerDesc& RHS) const {
            return  // strcmp(Name, RHS.Name) == 0 &&
                MinFilter == RHS.MinFilter      &&
                MagFilter == RHS.MagFilter      &&
                MipFilter == RHS.MipFilter      &&
                AddressU == RHS.AddressU       &&
                AddressV == RHS.AddressV       &&
                AddressW == RHS.AddressW       &&
                MipLODBias == RHS.MipLODBias     &&
                MaxAnisotropy == RHS.MaxAnisotropy  &&
                ComparisonFunc == RHS.ComparisonFunc &&
                BorderColor[0] == RHS.BorderColor[0] &&
                BorderColor[1] == RHS.BorderColor[1] &&
                BorderColor[2] == RHS.BorderColor[2] &&
                BorderColor[3] == RHS.BorderColor[3] &&
                MinLOD == RHS.MinLOD         &&
                MaxLOD == RHS.MaxLOD;
        }
    };

    class ISampler : public IDeviceObject {
    public:
        virtual const SamplerDesc& GetDesc()const = 0;
    };
}