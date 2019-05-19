#pragma once

#include <GSZHU/SLayoutElement.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    // This structure is used by IRenderDevice::CreatePipelineState().
    struct SInputLayoutDesc {
        const SLayoutElement* ElementArray = nullptr;
        uint32_t NumElements = 0;

        SInputLayoutDesc() noexcept {}
        SInputLayoutDesc(const SLayoutElement* _Elements,  uint32_t _NumElements) noexcept
            : ElementArray(_Elements)
            , NumElements(_NumElements)
        {}

        bool operator==(const SInputLayoutDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SInputLayoutDesc> {
        size_t operator()(const GSZHU::SInputLayoutDesc& Desc) const {
            return GSZHU::ComputeArrayHash<GSZHU::SLayoutElement>(Desc.ElementArray, Desc.NumElements);
        }
    };
}