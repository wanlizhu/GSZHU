#pragma once

#include <GSZHU/BasicTypes.h>
#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/ENUM/EResourceDimension.h>
#include <GSZHU/ENUM/ETextureFormat.h>
#include <GSZHU/ENUM/ETextureViewType.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct GSZHU_API STextureViewDesc : public SDeviceObjectAttribs {
        ETEXTURE_VIEW_TYPE ViewType = TEXTURE_VIEW_UNDEFINED;
        ERESOURCE_DIMENSION TextureDim = RESOURCE_DIM_UNDEFINED;
        ETEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
        uint32_t MostDetailedMip = 0;
        uint32_t NumMipLevels = 0;
        uint32_t FirstArraySlice = 0;
        uint32_t FirstDepthSlice = 0;
        uint32_t NumArraySlices = 0;
        uint32_t NumDepthSlices = 0;
        uint32_t AccessFlags = 0;

        STextureViewDesc() noexcept;
        STextureViewDesc(ETEXTURE_VIEW_TYPE _ViewType, ERESOURCE_DIMENSION _TextureDim) noexcept;

        bool operator==(const STextureViewDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::STextureViewDesc> {
        size_t operator()(const GSZHU::STextureViewDesc& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.ViewType),
                                      static_cast<int>(Desc.TextureDim),
                                      static_cast<int>(Desc.Format),
                                      Desc.MostDetailedMip,
                                      Desc.NumMipLevels,
                                      Desc.FirstArraySlice,
                                      Desc.FirstDepthSlice,
                                      Desc.NumArraySlices,
                                      Desc.NumDepthSlices,
                                      Desc.AccessFlags);
        }
    };
}