#pragma once

#include <GSZHU/Constants.h>
#include <GSZHU/ENUM/EValueType.h>

namespace GSZHU {
    struct SLayoutElement {
        enum EFREQUENCY : INT {
            FREQUENCY_UNDEFINED = 0,
            FREQUENCY_PER_VERTEX,
            FREQUENCY_PER_INSTANCE,
            FREQUENCY_NUM_FREQUENCIES
        };

        UINT InputIndex = 0;
        UINT BufferSlot = 0;
        UINT NumComponents = 0;
        EVALUE_TYPE ValueType = VT_FLOAT32;
        bool IsNormalized = true;
        UINT RelativeOffset = AUTO_OFFSET;
        UINT Stride = AUTO_STRIDE;
        EFREQUENCY Frequency = FREQUENCY_PER_VERTEX;
        UINT InstanceDataStepRate = 1;

        SLayoutElement() noexcept {}
        SLayoutElement(UINT _InputIndex, UINT _BufferSlot, UINT _NumComponents, EVALUE_TYPE _ValueType) noexcept
            : InputIndex(_InputIndex)
            , BufferSlot(_BufferSlot)
            , NumComponents(_NumComponents)
            , ValueType(_ValueType)
        {}
    };
}