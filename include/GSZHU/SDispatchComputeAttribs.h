#pragma once

#include <GSZHU/ENUM/EResourceStateTransitionMode.h>
#include <GSZHU/IBuffer.h>
#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // This structure is used by IDeviceContext::DispatchCompute().
    struct SDispatchComputeAttribs {
        UINT ThreadGroupCountX = 1; // Number of groups dispatched in X direction.
        UINT ThreadGroupCountY = 1; // Number of groups dispatched in Y direction.
        UINT ThreadGroupCountZ = 1; // Number of groups dispatched in Z direction.

        IBuffer* IndirectDispatchAttribs = nullptr;
        UINT  DispatchArgsByteOffset = 0;
        ERESOURCE_STATE_TRANSITION_MODE IndirectAttribsBufferStateTransitionMode = RESOURCE_STATE_TRANSITION_MODE_NONE;

        SDispatchComputeAttribs() noexcept {}
        explicit SDispatchComputeAttribs(UINT GroupsX, UINT GroupsY = 1, UINT GroupsZ = 1) noexcept 
            : ThreadGroupCountX(GroupsX)
            , ThreadGroupCountY(GroupsY)
            , ThreadGroupCountZ(GroupsZ)
        {}

        SDispatchComputeAttribs(IBuffer* DispatchAttribs, ERESOURCE_STATE_TRANSITION_MODE StateTransitionMode, UINT Offset = 0)
            : IndirectDispatchAttribs(DispatchAttribs)
            , DispatchArgsByteOffset(Offset)
            , IndirectAttribsBufferStateTransitionMode(StateTransitionMode)
        {}
    };
}