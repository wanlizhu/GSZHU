#pragma once

#include <GSZHU/BasicTypes.h>
#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/ENUM/EResourceDimension.h>
#include <GSZHU/ENUM/ETextureFormat.h>
#include <GSZHU/ENUM/ETextureViewType.h>

namespace GSZHU {
    struct STextureViewDesc : public SDeviceObjectAttribs {
        ETEXTURE_VIEW_TYPE ViewType = TEXTURE_VIEW_UNDEFINED;
        ERESOURCE_DIMENSION TextureDim = RESOURCE_DIM_UNDEFINED;
        ETEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
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

        STextureViewDesc() noexcept {}
        STextureViewDesc(ETEXTURE_VIEW_TYPE _ViewType, ERESOURCE_DIMENSION _TextureDim) noexcept 
            : ViewType(_ViewType)
            , TextureDim(_TextureDim)
        {}

        bool operator == (const STextureViewDesc& rhs) const {
            return ViewType == rhs.ViewType    
                && TextureDim == rhs.TextureDim   
                && Format == rhs.Format      
                && MostDetailedMip == rhs.MostDetailedMip
                && NumMipLevels == rhs.NumMipLevels  
                && FirstArraySlice == rhs.FirstArraySlice 
                && FirstDepthSlice == rhs.FirstDepthSlice 
                && NumArraySlices == rhs.NumArraySlices  
                && NumDepthSlices == rhs.NumDepthSlices 
                && AccessFlags == rhs.AccessFlags;
        }
    };
}