#pragma once

#include <GSZHU/DESC/SSamplerDesc.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct SStaticSamplerDesc{
        const char* SamplerName = nullptr;
        SSamplerDesc SamplerDesc;

        SStaticSamplerDesc()noexcept;
        SStaticSamplerDesc(const char* _SamplerName, const SSamplerDesc& _SamplerDesc) noexcept;

        bool operator==(const SStaticSamplerDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SStaticSamplerDesc> {
        size_t operator()(const GSZHU::SStaticSamplerDesc& Desc) const {
            return GSZHU::ComputeHash(std::string(Desc.SamplerName),
                                      std::hash<GSZHU::SSamplerDesc>()(Desc.SamplerDesc));
        }
    };
}