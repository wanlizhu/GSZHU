#include <GSZHU/DESC/SStaticSamplerDesc.h>

namespace GSZHU {
    SStaticSamplerDesc::SStaticSamplerDesc() noexcept
    {}

    SStaticSamplerDesc::SStaticSamplerDesc(const char* _SamplerName, const SSamplerDesc& _SamplerDesc) noexcept
        : SamplerName(_SamplerName)
        , SamplerDesc(_SamplerDesc)
    {}

    bool SStaticSamplerDesc::operator==(const SStaticSamplerDesc& rhs) const{
        return strcmp(SamplerName, rhs.SamplerName) == 0
            && SamplerDesc == rhs.SamplerDesc; 
    }
}