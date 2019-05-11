#include <GSZHU/SBufferFormat.h>

namespace GSZHU {
    SBufferFormat::SBufferFormat(EVALUE_TYPE _ValueType, UINT8 _NumComponents) noexcept 
        : ValueType(_ValueType)
        , NumComponents(_NumComponents)
    {}

    bool SBufferFormat::operator==(const SBufferFormat& rhs) const {
        return ValueType == rhs.ValueType 
            && NumComponents == rhs.NumComponents 
            && IsNormalized == rhs.IsNormalized;
    }
}