#pragma once

#include "IDeviceObject.h"

namespace Wanlix
{
    struct SamplerDesc : public DeviceObjectDesc
    {
        FilterType minFilter = FilterType::Linear;
        FilterType magFilter = FilterType::Linear;
        FilterType mipFilter = FilterType::Linear;
        AddressMode addressU = AddressMode::Clamp;
        AddressMode addressV = AddressMode::Clamp;
        AddressMode addressW = AddressMode::Clamp;
        float mipLodBias = 0.f;
        Uint maxAnisotropy = 0;
        ComparisonFunc comparisonFunc = ComparisonFunc::Never;
        float borderColor[4] = { 0.f, 0.f, 0.f, 1.f };
        float minLod = 0.f;
        float maxLod = std::numeric_limits<float>::max();
    };

    struct StaticSamplerDesc
    {
        Uint shaderStages = ShaderStages::Unknown;
        String name;
        SamplerDesc samplerDesc;
    };

    class ISampler : public inherit_shared_from_this<IDeviceObject, ISampler>
    {
    public:
        virtual ~ISampler() = default;

        /* interfaces inherited from IDeviceObject */
        virtual Pvoid GetNativeHandle() const = 0;
        virtual SamplerDesc const& GetDesc() const override = 0;
        virtual IDevice* GetDevice() const override = 0;
    };
}