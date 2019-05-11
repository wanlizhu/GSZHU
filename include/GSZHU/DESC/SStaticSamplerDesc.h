#pragma once

#include <GSZHU/DESC/SSamplerDesc.h>

namespace GSZHU {
    struct SStaticSamplerDesc{
        const char* SamplerOrTextureName = nullptr;
        SSamplerDesc SamplerDesc;

        SStaticSamplerDesc()noexcept {}
        SStaticSamplerDesc(const char* _SamplerOrTextureName, const SSamplerDesc& _SamplerDesc) noexcept 
            : SamplerOrTextureName(_SamplerOrTextureName)
            , SamplerDesc(_SamplerDesc)
        {}
    };

}