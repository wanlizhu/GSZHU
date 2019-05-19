#include <GSZHU/DESC/STextureViewDesc.h>

namespace GSZHU {
    STextureViewDesc::STextureViewDesc() noexcept
    {}

    STextureViewDesc::STextureViewDesc(ETEXTURE_VIEW_TYPE _ViewType, ERESOURCE_DIMENSION _TextureDim) noexcept
        : ViewType(_ViewType)
        , TextureDim(_TextureDim)
    {}

    bool STextureViewDesc::operator==(const STextureViewDesc& rhs) const {
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
}