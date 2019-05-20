#include <GSZHU/SDrawAttribs.h>

namespace GSZHU {
    SDrawAttribs::SDrawAttribs() noexcept 
    {}

    SDrawAttribs::SDrawAttribs(uint32_t _NumVertices, EDRAW_FLAGS _Flags) noexcept
        : NumVertices(_NumVertices)
        , DrawFlags(_Flags)
    {}
        
    SDrawAttribs::SDrawAttribs(uint32_t _NumIndices, EVALUE_TYPE  _IndexType, EDRAW_FLAGS  _Flags) noexcept 
        : NumIndices(_NumIndices)
        , IsIndexed(true)
        , IndexType(_IndexType)
        , DrawFlags(_Flags)
    {}

    SDrawAttribs::SDrawAttribs(IBuffer* _IndirectDrawAttribs, EDRAW_FLAGS _Flags, ERESOURCE_STATE_TRANSITION_MODE _IndirectAttribsBufferStateTransitionMode) noexcept 
        : DrawFlags(_Flags)
        , IndirectAttribsBufferStateTransitionMode(_IndirectAttribsBufferStateTransitionMode)
        , IndirectDrawAttribs(_IndirectDrawAttribs)
    {}

    SDrawAttribs::SDrawAttribs(IBuffer* _pIndirectDrawAttribs, EVALUE_TYPE _IndexType, EDRAW_FLAGS _Flags, ERESOURCE_STATE_TRANSITION_MODE _IndirectAttribsBufferStateTransitionMode) noexcept 
        : IsIndexed(true)
        , IndexType(_IndexType)
        , DrawFlags(_Flags)
        , IndirectAttribsBufferStateTransitionMode(_IndirectAttribsBufferStateTransitionMode)
    {}
}