#pragma once

#include "IResourceDesc.h"

namespace GSZHU {
    class CSamplerDesc : public IResourceDesc {
    public:
        CSamplerDesc(EFilter filter = EFilter::Linear,
            EAddressMode addressMode = EAddressMode::Clamp)
            : IResourceDesc(typeid(CSamplerDesc).name())
            , mMinFilter(filter)
            , mMagFilter(filter)
            , mMipFilter(filter)
            , mAddressU(addressMode)
            , mAddressV(addressMode)
            , mAddressW(addressMode)
        {}


    private:
        EFilter mMinFilter = EFilter::Linear;
        EFilter mMagFilter = EFilter::Linear;
        EFilter mMipFilter = EFilter::Linear;
        EAddressMode mAddressU = EAddressMode::Clamp;
        EAddressMode mAddressV = EAddressMode::Clamp;
        EAddressMode mAddressW = EAddressMode::Clamp;
        float mMipLODBias = 0;
        uint32_t mMaxAnisotropy = 0;
        float mBorderColor[4] = { 0,0,0,0 };
        float mMinLOD = 0;
        float mMaxLOD = std::numeric_limits<float>::max();
    };
}