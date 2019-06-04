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

        EFilter GetMinFilter() const { return mMinFilter; }
        EFilter GetMagFilter() const { return mMagFilter; }
        EFilter GetMipFilter() const { return mMinFilter; }
        EAddressMode GetAddressU() const { return mAddressU; }
        EAddressMode GetAddressV() const { return mAddressV; }
        EAddressMode GetAddressW() const { return mAddressW; }
        float GetMipLODBias() const { return mMipLODBias; }
        uint32_t GetMaxAnisotropy() const { return mMaxAnisotropy; }
        float const* GetBorderColor() const { return mBorderColor; }
        float GetMinLOD() const { return mMinLOD; }
        float GetMaxLOD() const { return mMaxLOD; }

        void SetMipLODBias(float bias) { mMipLODBias = bias; }
        void SetMaxAnisotropy(uint32_t max) { mMaxAnisotropy = max; }
        void SetBorderColor(float* color) { std::memmove(mBorderColor, color, 4 * sizeof(float)); }
        void SetMinLOD(float lod) { mMinLOD = lod; }
        void SetMaxLOD(float lod) { mMaxLOD = lod; }

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