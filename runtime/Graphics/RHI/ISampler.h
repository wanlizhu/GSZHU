#pragma once

#include "Graphics/RHI/IResource.h"

namespace Wanlix
{
    struct SamplerDesc : public ResourceDesc
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
        // Interfaces inherited from IResource
        virtual EResourceType GetType() const override { return EResourceType::Sampler; }

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const SamplerDesc& GetDesc() const = 0;
    };
}