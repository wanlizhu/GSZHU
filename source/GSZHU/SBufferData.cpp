#include <GSZHU/SBufferData.h>

namespace GSZHU {
    SBufferData::SBufferData() noexcept 
    {}

    SBufferData::SBufferData(const void* _Data, uint32_t _Size) noexcept
        : Data(_Data)
        , Size(_Size) 
    {}
}