#pragma once

#include <NTGS/Graphics/IDeviceObject.h>

namespace NTGS {
    class IDeviceContext;
    class ISampler;

    enum UAV_ACCESS_FLAG : INT {
        UAV_ACCESS_UNSPECIFIED = 0x00,
        UAV_ACCESS_FLAG_READ = 0x01,
        UAV_ACCESS_FLAG_WRITE = 0x02,
        UAV_ACCESS_FLAG_READ_WRITE = UAV_ACCESS_FLAG_READ | UAV_ACCESS_FLAG_WRITE
    };

    struct TextureViewDesc : public IDeviceObjectAttribs {
        static constexpr INT RemainingMipLevels = static_cast<INT>(-1);
        static constexpr INT RemainingArraySlices = static_cast<INT>(-1);

        TEXTURE_VIEW_TYPE ViewType = TEXTURE_VIEW_UNDEFINED;
        RESOURCE_DIMENSION TextureDim = RESOURCE_DIM_UNDEFINED;
        TEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
        UINT MostDetailedMip = 0;
        UINT NumMipLevels = 0;
        union {
            UINT FirstArraySlice = 0;
            UINT FirstDepthSlice;
        };
        union {
            UINT NumArraySlices = 0;
            UINT NumDepthSlices;
        };
        UINT AccessFlags = 0;

        TextureViewDesc() {}
        TextureViewDesc(TEXTURE_VIEW_TYPE _ViewType,
                        RESOURCE_DIMENSION _TextureDim,
                        TEXTURE_FORMAT _Format = TextureViewDesc{}.Format,
                        UINT _MostDetailedMip = TextureViewDesc{}.MostDetailedMip,
                        UINT _NumMipLevels = TextureViewDesc{}.NumMipLevels,
                        UINT _FirstArrayOrDepthSlice = TextureViewDesc{}.FirstArraySlice,
                        UINT _NumArrayOrDepthSlices = TextureViewDesc{}.NumArraySlices,
                        UINT _AccessFlags = TextureViewDesc{}.AccessFlags) noexcept
            : ViewType(_ViewType),
              TextureDim(_TextureDim),
              Format(_Format),
              MostDetailedMip(_MostDetailedMip),
              NumMipLevels(_NumMipLevels),
              FirstArraySlice(_FirstArrayOrDepthSlice),
              NumArraySlices(_NumArrayOrDepthSlices),
              AccessFlags(_AccessFlags) {}

        bool operator == (const TextureViewDesc& RHS) const {
            return //strcmp(Name, RHS.Name) == 0 &&
                ViewType == RHS.ViewType        &&
                TextureDim == RHS.TextureDim      &&
                Format == RHS.Format          &&
                MostDetailedMip == RHS.MostDetailedMip &&
                NumMipLevels == RHS.NumMipLevels    &&
                FirstArraySlice == RHS.FirstArraySlice &&
                FirstDepthSlice == RHS.FirstDepthSlice &&
                NumArraySlices == RHS.NumArraySlices  &&
                NumDepthSlices == RHS.NumDepthSlices  &&
                AccessFlags == RHS.AccessFlags;
        }
    };

    class ITextureView : public IDeviceObject {
    public:
        virtual const TextureViewDesc& GetDesc() const = 0;
        virtual void SetSampler(ISampler *pSampler) = 0;
        virtual ISampler* GetSampler() = 0;
        virtual class ITexture* GetTexture() = 0;
    };
}