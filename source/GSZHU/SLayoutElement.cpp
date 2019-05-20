#include <GSZHU/SLayoutElement.h>

namespace GSZHU {
    SLayoutElement::SLayoutElement() noexcept 
    {}

    SLayoutElement::SLayoutElement(uint32_t _InputIndex, uint32_t _BufferSlot, uint32_t _NumComponents, EVALUE_TYPE _ValueType) noexcept
        : InputIndex(_InputIndex)
        , BufferSlot(_BufferSlot)
        , NumComponents(_NumComponents)
        , ValueType(_ValueType)
    {}

    bool SLayoutElement::operator==(const SLayoutElement& rhs) const {
        return InputIndex == rhs.InputIndex
            && BufferSlot == rhs.BufferSlot
            && NumComponents == rhs.NumComponents
            && ValueType == rhs.ValueType
            && IsNormalized == rhs.IsNormalized
            && RelativeOffset == rhs.RelativeOffset
            && Stride == rhs.Stride
            && Frequency == rhs.Frequency
            && InstanceDataStepRate == rhs.InstanceDataStepRate;
    }
}