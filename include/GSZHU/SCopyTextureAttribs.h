#pragma once

#include <GSZHU/ITexture.h>
#include <GSZHU/BasicTypes.h>
#include <GSZHU/ENUM/EResourceStateTransitionMode.h>

namespace GSZHU {
    // This structure is used by IDeviceContext::CopyTexture().
    struct SCopyTextureAttribs {
        ITexture* SrcTexture = nullptr;
        UINT SrcMipLevel = 0;
        UINT SrcSlice = 0;
        const Box3D* SrcBox = nullptr;
        ERESOURCE_STATE_TRANSITION_MODE SrcTextureTransitionMode = RESOURCE_STATE_TRANSITION_MODE_NONE;

        ITexture* DstTexture = nullptr;
        UINT DstMipLevel = 0;
        UINT DstSlice = 0;      
        UINT DstX = 0;
        UINT DstY = 0;
        UINT DstZ = 0;
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