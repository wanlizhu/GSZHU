#pragma once

#include "RHI/IResource.h"

namespace Wanlix
{
    struct SamplerDesc : public DeviceObjectDesc
    {
        EFilterType minFilter = EFilterType::Linear;
        EFilterType magFilter = EFilterType::Linear;
        EFilterType mipFilter = EFilterType::Linear;
        EAddressMode addressU = EAddressMode::Clamp;
        EAddressMode addressV = EAddressMode::Clamp;
        EAddressMode addressW = EAddressMode::Clamp;
        float mipLodBias = 0.f;
        Uint maxAnisotropy = 0;
        EComparisonFunc comparisonFunc = EComparisonFunc::Always;
        Float4 borderColor = Float4(0, 0, 0, 0);
        float minLod = 0.f;
        float maxLod = std::numeric_limits<float>::max();
    };

    struct StaticSampler
    {
        String name;
        SamplerDesc desc;
    };

    class ISampler : public IResource
    {
    public:
        using Desc = SamplerDesc;

        virtual const SamplerDesc& GetDesc() const = 0;
    };
}