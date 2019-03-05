#pragma once

#include <NTGS/Graphics/GraphicsEnums.h>

namespace NTGS {
    struct LayoutElement {
        enum FREQUENCY : INT {
            FREQUENCY_UNDEFINED = 0,
            FREQUENCY_PER_VERTEX,
            FREQUENCY_PER_INSTANCE,
            FREQUENCY_NUM_FREQUENCIES
        };
        static constexpr UINT AutoOffset = static_cast<UINT>(-1);
        static constexpr UINT AutoStride = static_cast<UINT>(-1);

        UINT InputIndex = 0;
        UINT BufferSlot = 0;
        UINT NumComponents = 0;
        VALUE_TYPE ValueType = VT_FLOAT32;
        bool IsNormalized = true;
        UINT RelativeOffset = AutoOffset;
        UINT Stride = AutoOffset;
        FREQUENCY Frequency = FREQUENCY_PER_VERTEX;
        UINT InstanceDataStepRate = 1;

        LayoutElement() = default;
        LayoutElement(UINT _InputIndex,
                      UINT _BufferSlot,
                      UINT _NumComponents,
                      VALUE_TYPE _ValueType,
                      bool _IsNormalized = LayoutElement{}.IsNormalized,
                      UINT _RelativeOffset = LayoutElement{}.RelativeOffset,
                      UINT _Stride = LayoutElement{}.Stride,
                      FREQUENCY _Frequency = LayoutElement{}.Frequency,
                      UINT _InstanceDataStepRate = LayoutElement{}.InstanceDataStepRate) noexcept 
            : InputIndex(_InputIndex),
              BufferSlot(_BufferSlot),
              NumComponents(_NumComponents),
              ValueType(_ValueType),
              IsNormalized(_IsNormalized),
              RelativeOffset(_RelativeOffset),
              Stride(_Stride),
              Frequency(_Frequency),
              InstanceDataStepRate(_InstanceDataStepRate) {}
    };

    struct InputLayoutDesc {
        const LayoutElement*  LayoutElements = nullptr;
        UINT NumElements = 0;

        InputLayoutDesc() = default;
        InputLayoutDesc(const LayoutElement* _LayoutElements,
                        UINT _NumElements) noexcept 
            : LayoutElements(_LayoutElements),
              NumElements(_NumElements) {}
    };
}