#include <GSZHU/SCopyTextureAttribs.h>

namespace GSZHU {
    SCopyTextureAttribs::SCopyTextureAttribs() noexcept
    {}

    SCopyTextureAttribs::SCopyTextureAttribs(ITexture* _SrcTexture, ERESOURCE_STATE_TRANSITION_MODE _SrcTextureTransitionMode, 
                                             ITexture* _DstTexture, ERESOURCE_STATE_TRANSITION_MODE _DstTextureTransitionMode) noexcept 
        : SrcTexture(_SrcTexture)
        , SrcTextureTransitionMode(_SrcTextureTransitionMode)
        , DstTexture(_DstTexture)
        , DstTextureTransitionMode(_DstTextureTransitionMode)
    {}
}