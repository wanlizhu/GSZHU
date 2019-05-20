#pragma once

#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/SOptimizedClearValue.h>
#include <GSZHU/ENUM/EResourceState.h>
#include <GSZHU/ENUM/ETextureFormat.h>
#include <GSZHU/ENUM/EUsage.h>
#include <GSZHU/ENUM/EBindFlags.h>
#include <GSZHU/ENUM/ECPUAccessFlags.h>
#include <GSZHU/ENUM/EMiscTextureFlags.h>
#include <GSZHU/ENUM/EResourceDimension.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct GSZHU_API STextureDesc : public SDeviceObjectAttribs {
        ERESOURCE_DIMENSION Type = RESOURCE_DIM_UNDEFINED;
        uint32_t Width = 0;
        uint32_t Height = 0;
        uint32_t ArraySize = 1; // For a 1D array or 2D array, number of array slices
        uint32_t Depth = 0; // For a 3D texture, number of depth slices
        ETEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;

        uint32_t MipLevels = 1;
        uint32_t SampleCount = 1;
        EUSAGE Usage = USAGE_DEFAULT;
        EBIND_FLAGS BindFlags = BIND_NONE;
        ECPU_ACCESS_FLAGS CPUAccessFlags = CPU_ACCESS_NONE;
        EMISC_TEXTURE_FLAGS MiscFlags = MISC_TEXTURE_FLAG_NONE;
        SOptimizedClearValue ClearValue;
        uint64_t CommandQueueMask = 1;

        STextureDesc() noexcept;
        STextureDesc(ERESOURCE_DIMENSION _Type,
                     uint32_t _Width, 
                     uint32_t _Height,
                     uint32_t _ArraySizeOrDepth,
                     ETEXTURE_FORMAT _Format) noexcept;

        bool operator==(const STextureDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::STextureDesc> {
        size_t operator()(const GSZHU::STextureDesc& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.Type),
                                      Desc.Width,
                                      Desc.Height,
                                      Desc.ArraySize,
                                      Desc.Depth,
                                      static_cast<int>(Desc.Format),
                                      Desc.MipLevels,
                                      Desc.SampleCount,
                                      static_cast<int>(Desc.Usage),
                                      static_cast<int>(Desc.CPUAccessFlags),
                                      static_cast<int>(Desc.MiscFlags),
                                      std::hash<GSZHU::SOptimizedClearValue>()(Desc.ClearValue),
                                      Desc.CommandQueueMask);
        }
    };
}