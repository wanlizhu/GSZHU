#pragma once

#include <GSZHU/ENUM/EResourceStateTransitionMode.h>
#include <GSZHU/IBuffer.h>
#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // This structure is used by IDeviceContext::DispatchCompute().
    struct SDispatchComputeAttribs {
        uint32_t ThreadGroupCountX = 1; // Number of groups dispatched in X direction.
        uint32_t ThreadGroupCountY = 1; // Number of groups dispatched in Y direction.
        uint32_t ThreadGroupCountZ = 1; // Number of groups dispatched in Z direction.

        IBuffer* IndirectDispatchAttribs = nullptr;
        uint32_t  DispatchArgsByteOffset = 0;
        ERESOURCE_STATE_TRANSITION_MODE IndirectAttribsBufferStateTransitionMode = RESOURCE_STATE_TRANSITION_MODE_NONE;

        SDispatchComputeAttribs() noexcept {}
        explicit SDispatchComputeAttribs(uint32_t GroupsX, uint32_t GroupsY = 1, uint32_t GroupsZ = 1) noexcept 
            : ThreadGroupCountX(GroupsX)
            , ThreadGroupCountY(GroupsY)
            , ThreadGroupCountZ(GroupsZ)
        {}

        SDispatchComputeAttribs(IBuffer* DispatchAttribs, ERESOURCE_STATE_TRANSITION_MODE StateTransitionMode, uint32_t Offset = 0)
            : IndirectDispatchAttribs(DispatchAttribs)
            , DispatchArgsByteOffset(Offset)
            , IndirectAttribsBufferStateTransitionMode(StateTransitionMode)
        {}
    };
}