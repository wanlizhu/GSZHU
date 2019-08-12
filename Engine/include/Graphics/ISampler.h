#pragma once

#include "Graphics/IDeviceObject.h"

namespace ZHU
{
    struct SamplerDesc : public DeviceObjectDesc
    {
        EFilter MinFilter = EFilter::Linear;
        EFilter MagFilter = EFilter::Linear;
        EFilter MipFilter = EFilter::Linear;

        EAddressMode AddressU = EAddressMode::Clamp;
        EAddressMode AddressV = EAddressMode::Clamp;
        EAddressMode AddressW = EAddressMode::Clamp;

        float MipLODBias = 0;
        uint32_t MaxAnisotropy = 0;
        EComparisonFunction ComparisonFunction = EComparisonFunction::Never;
        float BorderColor[4] = { 0.f, 0.f, 0.f, 0.f };
        float MinLOD = 0;
        float MaxLOD = std::numeric_limits<float>::max();
    };


    class ZHU_API ISampler : public IDeviceObject
    {
    public:
        virtual const SamplerDesc& GetDesc() const = 0;
    };
}