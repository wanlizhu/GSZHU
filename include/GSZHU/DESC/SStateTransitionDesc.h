#pragma once

#include <GSZHU/IBuffer.h>
#include <GSZHU/BasicTypes.h>
#include <GSZHU/ENUM/EResourceState.h>
#include <GSZHU/ITexture.h>
#include <GSZHU/ENUM/EStateTransitionType.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    // Resource state transition barrier description
    struct SStateTransitionDesc {
        ITexture* Texture = nullptr;
        IBuffer* Buffer = nullptr;
        uint32_t FirstMipLevel = 0;
        uint32_t MipLevelsCount = (uint32_t)-1;
        uint32_t FirstArraySlice = 0;
        uint32_t ArraySliceCount = (uint32_t)-1;
        ERESOURCE_STATE OldState = RESOURCE_STATE_UNKNOWN;
        ERESOURCE_STATE NewState = RESOURCE_STATE_UNKNOWN;
        ESTATE_TRANSITION_TYPE TransitionType = STATE_TRANSITION_TYPE_IMMEDIATE;
        bool UpdateResourceState = false;

        SStateTransitionDesc() noexcept;
        bool operator==(const SStateTransitionDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SStateTransitionDesc> {
        size_t operator()(const GSZHU::SStateTransitionDesc& Desc) const {
            return GSZHU::ComputeHash(Desc.Texture,
                                      Desc.Buffer,
                                      Desc.FirstMipLevel,
                                      Desc.MipLevelsCount,
                                      Desc.FirstArraySlice,
                                      Desc.ArraySliceCount,
                                      static_cast<int>(Desc.OldState),
                                      static_cast<int>(Desc.NewState),
                                      static_cast<int>(Desc.TransitionType),
                                      static_cast<int>(Desc.UpdateResourceState));
        }
    };
}