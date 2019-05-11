#pragma once

#include <GSZHU/SLayoutElement.h>

namespace GSZHU {
    // This structure is used by IRenderDevice::CreatePipelineState().
    struct SInputLayoutDesc {
        const SLayoutElement* LayoutElements = nullptr;
        UINT NumElements = 0;

        SInputLayoutDesc() noexcept {}
        SInputLayoutDesc(const SLayoutElement* _LayoutElements,  UINT _NumElements) noexcept
            : LayoutElements(_LayoutElements)
            , NumElements(_NumElements)
        {}
    };
}