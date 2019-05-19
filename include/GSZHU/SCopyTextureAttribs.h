#pragma once

#include <GSZHU/ITexture.h>
#include <GSZHU/BasicTypes.h>
#include <GSZHU/ENUM/EResourceStateTransitionMode.h>

namespace GSZHU {
    // This structure is used by IDeviceContext::CopyTexture().
    struct SCopyTextureAttribs {
        ITexture* SrcTexture = nullptr;
        uint32_t SrcMipLevel = 0;
        uint32_t SrcSlice = 0;
        const Box3D* SrcBox = nullptr;
        ERESOURCE_STATE_TRANSITION_MODE SrcTextureTransitionMode = RESOURCE_STATE_TRANSITION_MODE_NONE;

        ITexture* DstTexture = nullptr;
        uint32_t DstMipLevel = 0;
        uint32_t DstSlice = 0;      
        uint32_t DstX = 0;
        uint32_t DstY = 0;
        uint32_t DstZ = 0;
        ERESOURCE_STATE_TRANSITION_MODE DstTextureTransitionMode = RESOURCE_STATE_TRANSITION_MODE_NONE;

        SCopyTextureAttribs() noexcept {}
        SCopyTextureAttribs(ITexture* _SrcTexture, ERESOURCE_STATE_TRANSITION_MODE _SrcTextureTransitionMode, 
                            ITexture* _DstTexture, ERESOURCE_STATE_TRANSITION_MODE _DstTextureTransitionMode) noexcept 
            : SrcTexture(_SrcTexture)
            , SrcTextureTransitionMode(_SrcTextureTransitionMode)
            , DstTexture(_DstTexture)
            , DstTextureTransitionMode(_DstTextureTransitionMode)
        {}
    };
}