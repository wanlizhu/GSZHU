#include <GSZHU/DESC/SStateTransitionDesc.h>

namespace GSZHU {
    SStateTransitionDesc::SStateTransitionDesc() noexcept
    {}

    bool SStateTransitionDesc::operator==(const SStateTransitionDesc& rhs) const {
        return Texture == rhs.Texture
            && Buffer == rhs.Buffer
            && FirstMipLevel == rhs.FirstMipLevel
            && MipLevelsCount == rhs.MipLevelsCount
            && FirstArraySlice == rhs.FirstArraySlice
            && ArraySliceCount == rhs.ArraySliceCount
            && OldState == rhs.OldState
            && NewState == rhs.NewState
            && TransitionType == rhs.TransitionType
            && UpdateResourceState == rhs.UpdateResourceState;
    }
}