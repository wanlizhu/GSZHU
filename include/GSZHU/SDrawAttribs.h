#pragma once

#include <GSZHU/ENUM/EDrawFlags.h>
#include <GSZHU/ENUM/EResourceStateTransitionMode.h>
#include <GSZHU/BasicTypes.h>
#include <GSZHU/ENUM/EValueType.h>
#include <GSZHU/IBuffer.h>

namespace GSZHU {
    // This structure is used by IRenderDevice::Draw().
    struct GSZHU_API SDrawAttribs {
        union {
            uint32_t NumVertices = 0; // For a non-indexed draw call
            uint32_t NumIndices; // For an indexed draw call
        };
        bool IsIndexed = false;
        EVALUE_TYPE IndexType = VT_UNDEFINED;

        EDRAW_FLAGS DrawFlags = DRAW_FLAG_NONE;
        ERESOURCE_STATE_TRANSITION_MODE IndirectAttribsBufferStateTransitionMode = RESOURCE_STATE_TRANSITION_MODE_NONE;

        uint32_t NumInstances = 1;
        uint32_t BaseVertex = 0;
        uint32_t IndirectDrawArgsOffset = 0;

        union {
            uint32_t StartVertexLocation = 0;
            uint32_t FirstIndexLocation;
        };
        uint32_t FirstInstanceLocation = 0;
        IBuffer* IndirectDrawAttribs = nullptr;

        SDrawAttribs() noexcept;
        SDrawAttribs(uint32_t _NumVertices, EDRAW_FLAGS _Flags) noexcept;
        SDrawAttribs(uint32_t _NumIndices, EVALUE_TYPE  _IndexType, EDRAW_FLAGS  _Flags) noexcept;
        SDrawAttribs(IBuffer* _IndirectDrawAttribs, EDRAW_FLAGS _Flags, ERESOURCE_STATE_TRANSITION_MODE _IndirectAttribsBufferStateTransitionMode) noexcept;
        SDrawAttribs(IBuffer* _pIndirectDrawAttribs, EVALUE_TYPE _IndexType, EDRAW_FLAGS _Flags, ERESOURCE_STATE_TRANSITION_MODE _IndirectAttribsBufferStateTransitionMode) noexcept;
    };

}