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

namespace GSZHU {
    struct STextureDesc : public SDeviceObjectAttribs {
        ERESOURCE_DIMENSION Type = RESOURCE_DIM_UNDEFINED;
        UINT Width = 0;
        UINT Height = 0;
        union {
            UINT ArraySize = 1; // For a 1D array or 2D array, number of array slices
            UINT Depth; // For a 3D texture, number of depth slices
        };
        ETEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;

        UINT MipLevels = 1;
        UINT SampleCount = 1;
        EUSAGE Usage = USAGE_DEFAULT;
        EBIND_FLAGS BindFlags = BIND_NONE;
        ECPU_ACCESS_FLAGS CPUAccessFlags = CPU_ACCESS_NONE;
        EMISC_TEXTURE_FLAGS MiscFlags = MISC_TEXTURE_FLAG_NONE;
        SOptimizedClearValue ClearValue;
        UINT64 CommandQueueMask = 1;

        STextureDesc() noexcept {}
        STextureDesc(ERESOURCE_DIMENSION  _Type, UINT _Width, UINT _Height, UINT _ArraySizeOrDepth, ETEXTURE_FORMAT _Format) 
            : Type(_Type)
            , Width(_Width)
            , Height(_Height)
            , ArraySize(_ArraySizeOrDepth)
            , Format(_Format)
        {}

        bool operator==(const STextureDesc& rhs) const {
            return Type == rhs.Type
                && Width == rhs.Width
                && Height == rhs.Height
                && ArraySize == rhs.ArraySize
                && Format == rhs.Format
                && MipLevels == rhs.MipLevels
                && SampleCount == rhs.SampleCount
                && Usage == rhs.Usage
                && BindFlags == rhs.BindFlags
                && CPUAccessFlags == rhs.CPUAccessFlags 
                && MiscFlags == rhs.MiscFlags
                && ClearValue == rhs.ClearValue
                && CommandQueueMask == rhs.CommandQueueMask;
        }
    };
}