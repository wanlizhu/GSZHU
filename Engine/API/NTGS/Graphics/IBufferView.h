#pragma once

#include <NTGS/Graphics/IBuffer.h>

namespace NTGS {
    struct BufferFormat {
        VALUE_TYPE ValueType = VT_UNDEFINED;
        UINT8 NumComponents = 0;
        bool IsNormalized = false;

        BufferFormat() = default;
        BufferFormat(VALUE_TYPE _ValueType,
                     UINT8 _NumComponents,
                     bool _IsNormalized = BufferFormat{}.IsNormalized) noexcept 
            : ValueType(_ValueType),
              NumComponents(_NumComponents),
              IsNormalized(_IsNormalized) {}

        bool operator == (const BufferFormat& RHS) const {
            return ValueType == RHS.ValueType &&
                NumComponents == RHS.NumComponents &&
                IsNormalized == RHS.IsNormalized;
        }
    };

    struct BufferViewDesc : IDeviceObjectAttribs {
        BUFFER_VIEW_TYPE ViewType = BUFFER_VIEW_UNDEFINED;
        BufferFormat Format;
        UINT ByteOffset = 0;
        UINT ByteWidth = 0;

        BufferViewDesc() = default;
        explicit BufferViewDesc(BUFFER_VIEW_TYPE _ViewType,
                                BufferFormat _Format = BufferViewDesc{}.Format,
                                UINT _ByteOffset = BufferViewDesc{}.ByteOffset,
                                UINT _ByteWidth = BufferViewDesc{}.ByteWidth) noexcept 
            : ViewType(_ViewType),
              Format(_Format),
              ByteOffset(_ByteOffset),
              ByteWidth(_ByteWidth) {}

        bool operator == (const BufferViewDesc& RHS) const {
            return //strcmp(Name, RHS.Name) == 0 &&
                ViewType == RHS.ViewType   &&
                ByteOffset == RHS.ByteOffset &&
                ByteWidth == RHS.ByteWidth  &&
                Format == RHS.Format;
        }
    };

    class IBufferView : public IDeviceObject {
    public:
        virtual const BufferViewDesc& GetDesc()const = 0;
        virtual IBuffer* GetBuffer() = 0;
    };
}