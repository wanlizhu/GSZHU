#pragma once

#include <GSZHU/IBuffer.h>
#include <GSZHU/BasicTypes.h>
#include <GSZHU/ENUM/EResourceState.h>
#include <GSZHU/ITexture.h>
#include <GSZHU/ENUM/EStateTransitionType.h>

namespace GSZHU {
    // Resource state transition barrier description
    struct SStateTransitionDesc {
        ITexture* Texture = nullptr;
        IBuffer* Buffer = nullptr;
        UINT FirstMipLevel = 0;
        UINT MipLevelsCount = (UINT)-1;
        UINT FirstArraySlice = 0;
        UINT ArraySliceCount = (UINT)-1;
        ERESOURCE_STATE OldState = RESOURCE_STATE_UNKNOWN;
        ERESOURCE_STATE NewState = RESOURCE_STATE_UNKNOWN;
        ESTATE_TRANSITION_TYPE TransitionType = STATE_TRANSITION_TYPE_IMMEDIATE;
        bool UpdateResourceState = false;

        SStateTransitionDesc() noexcept {}
        SStateTransitionDesc(ITexture* _Texture, ERESOURCE_STATE _OldState, ERESOURCE_STATE _NewState) noexcept 
            : Texture(_Texture)
            , OldState(_OldState)
            , NewState(_NewState)
        {}

        SStateTransitionDesc(ITexture* _Texture, ERESOURCE_STATE _OldState, ERESOURCE_STATE _NewState, bool _UpdateState) noexcept 
            : Texture(_Texture)
            , OldState(_OldState)
            , NewState(_NewState)
            , UpdateResourceState(_UpdateState)
        {}

        SStateTransitionDesc(IBuffer* _Buffer, ERESOURCE_STATE _OldState, ERESOURCE_STATE _NewState, bool _UpdateState) noexcept 
            : Buffer(_Buffer)
            , OldState(_OldState)
            , NewState(_NewState)
            , UpdateResourceState(_UpdateState)
        {}
    };
}