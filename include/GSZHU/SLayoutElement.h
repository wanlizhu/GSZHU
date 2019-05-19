#pragma once

#include <GSZHU/Constants.h>
#include <GSZHU/ENUM/EValueType.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct SLayoutElement {
        enum EFREQUENCY : int {
            FREQUENCY_UNDEFINED = 0,
            FREQUENCY_PER_VERTEX,
            FREQUENCY_PER_INSTANCE,
            FREQUENCY_NUM_FREQUENCIES
        };

        uint32_t InputIndex = 0;
        uint32_t BufferSlot = 0;
        uint32_t NumComponents = 0;
        EVALUE_TYPE ValueType = VT_FLOAT32;
        bool IsNormalized = true;
        uint32_t RelativeOffset = AUTO_OFFSET;
        uint32_t Stride = AUTO_STRIDE;
        EFREQUENCY Frequency = FREQUENCY_PER_VERTEX;
        uint32_t InstanceDataStepRate = 1;

        SLayoutElement() noexcept {}
        SLayoutElement(uint32_t _InputIndex, uint32_t _BufferSlot, uint32_t _NumComponents, EVALUE_TYPE _ValueType) noexcept
            : InputIndex(_InputIndex)
            , BufferSlot(_BufferSlot)
            , NumComponents(_NumComponents)
            , ValueType(_ValueType)
        {}
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SLayoutElement> {
        size_t operator()(const GSZHU::SLayoutElement& Element) const {
            return GSZHU::ComputeHash(Element.InputIndex,
                                      Element.BufferSlot,
                                      Element.NumComponents,
                                      static_cast<int>(Element.ValueType),
                                      static_cast<int>(Element.IsNormalized),
                                      Element.RelativeOffset,
                                      Element.Stride,
                                      static_cast<int>(Element.Frequency),
                                      Element.InstanceDataStepRate);
        }
    };
}