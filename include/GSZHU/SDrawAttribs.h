#pragma once

#include <GSZHU/ENUM/EDrawFlags.h>
#include <GSZHU/ENUM/EResourceStateTransitionMode.h>
#include <GSZHU/BasicTypes.h>
#include <GSZHU/ENUM/EValueType.h>
#include <GSZHU/IBuffer.h>

namespace GSZHU {
    // This structure is used by IRenderDevice::Draw().
    struct SDrawAttribs {
        union {
            UINT NumVertices = 0; // For a non-indexed draw call
            UINT NumIndices; // For an indexed draw call
        };
        bool IsIndexed = false;
        EVALUE_TYPE IndexType = VT_UNDEFINED;

        EDRAW_FLAGS DrawFlags = DRAW_FLAG_NONE;
        ERESOURCE_STATE_TRANSITION_MODE IndirectAttribsBufferStateTransitionMode = RESOURCE_STATE_TRANSITION_MODE_NONE;

        UINT NumInstances = 1;
        UINT BaseVertex = 0;
        UINT IndirectDrawArgsOffset = 0;

        union {
            UINT StartVertexLocation = 0;
            UINT FirstIndexLocation;
        };
        UINT FirstInstanceLocation = 0;
        IBuffer* IndirectDrawAttribs = nullptr;

        SDrawAttribs() noexcept {}
        SDrawAttribs(UINT _NumVertices, EDRAW_FLAGS _Flags) noexcept
            : NumVertices(_NumVertices)
            , DrawFlags(_Flags)
        {}
        SDrawAttribs(UINT _NumIndices, EVALUE_TYPE  _IndexType, EDRAW_FLAGS  _Flags) noexcept 
            : NumIndices(_NumIndices)
            , IsIndexed(true)
            , IndexType(_IndexType)
            , DrawFlags(_Flags)
        {}
        SDrawAttribs(IBuffer* _IndirectDrawAttribs, EDRAW_FLAGS _Flags, ERESOURCE_STATE_TRANSITION_MODE _IndirectAttribsBufferStateTransitionMode) noexcept 
            : DrawFlags(_Flags)
            , IndirectAttribsBufferStateTransitionMode(_IndirectAttribsBufferStateTransitionMode)
            , IndirectDrawAttribs(_IndirectDrawAttribs)
        {}
        SDrawAttribs(IBuffer* _pIndirectDrawAttribs, EVALUE_TYPE _IndexType, EDRAW_FLAGS _Flags, ERESOURCE_STATE_TRANSITION_MODE _IndirectAttribsBufferStateTransitionMode) noexcept 
            : IsIndexed(true)
            , IndexType(_IndexType)
            , DrawFlags(_Flags)
            , IndirectAttribsBufferStateTransitionMode(_IndirectAttribsBufferStateTransitionMode)
        {}
    };

}