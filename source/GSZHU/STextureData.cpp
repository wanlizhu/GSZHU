#include <GSZHU/STextureData.h>

namespace GSZHU {
    STextureData::STextureData() noexcept
    {}

    STextureData::STextureData(STextureSubresourceData* _Subres, uint32_t _NumSubres) noexcept
        : Subresources(_Subres)
        , NumSubresources(_NumSubres)
    {}
}