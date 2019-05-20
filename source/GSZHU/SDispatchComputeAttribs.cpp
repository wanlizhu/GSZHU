#include <GSZHU/SDispatchComputeAttribs.h>

namespace GSZHU {
    SDispatchComputeAttribs::SDispatchComputeAttribs() noexcept 
    {}

    SDispatchComputeAttribs::SDispatchComputeAttribs(uint32_t GroupsX, uint32_t GroupsY, uint32_t GroupsZ) noexcept 
        : ThreadGroupCountX(GroupsX)
        , ThreadGroupCountY(GroupsY)
        , ThreadGroupCountZ(GroupsZ)
    {}

    SDispatchComputeAttribs::SDispatchComputeAttribs(IBuffer* DispatchAttribs, ERESOURCE_STATE_TRANSITION_MODE StateTransitionMode, uint32_t Offset)
        : IndirectDispatchAttribs(DispatchAttribs)
        , DispatchArgsByteOffset(Offset)
        , IndirectAttribsBufferStateTransitionMode(StateTransitionMode)
    {}
}