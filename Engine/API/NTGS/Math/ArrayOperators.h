#pragma once

#include <NTGS/Core/Config.h>
#include <NTGS/Math/ArrayArithmeticOperators.h>
#include <NTGS/Math/ArrayComparisonOperators.h>
#include <assert.h>
#include <memory>
#include <type_traits>

namespace NTGS {

    template<typename ArrayType>
    class ArrayOperators : public ArrayArithmeticOperators<ArrayType>
                         , public ArrayComparisonOperators<ArrayType> {
    protected:
        inline void Fill(const typename ArrayType::Scalar& Value) noexcept {
            ArrayType& Me = static_cast<ArrayType&>(*this);
            for (int i = 0; i < DerivedType::DIMENSIONS; i++) {
                Me.mData[i] = Value;
            }
        }

        inline void Copy(const typename ArrayType::Scalar* Data) noexcept {
            ArrayType& Me = static_cast<ArrayType&>(*this);
            for (int i = 0; i < ArrayType::DIMENSIONS; i++) {
                Me.mData[i] = Data[i];
            }
        }

        template<typename = std::enable_if<ArrayType::DIMENSIONS >= 2>>
        inline void Set(const typename ArrayType::Scalar& Val0, const typename ArrayType::Scalar& Val1) noexcept {
            ArrayType& Me = static_cast<ArrayType&>(*this);
            Me[0] = Val0;
            Me[1] = Val1;
        }

        template<typename = std::enable_if<ArrayType::DIMENSIONS >= 3>>
        inline void Set(const typename ArrayType::Scalar& Val0, 
                        const typename ArrayType::Scalar& Val1,
                        const typename ArrayType::Scalar& Val2) noexcept {
            ArrayType& Me = static_cast<ArrayType&>(*this);
            Me[0] = Val0;
            Me[1] = Val1;
            Me[2] = Val2;
        }

        template<typename = std::enable_if<ArrayType::DIMENSIONS >= 4>>
        inline void Set(const typename ArrayType::Scalar& Val0,
                        const typename ArrayType::Scalar& Val1,
                        const typename ArrayType::Scalar& Val2,
                        const typename ArrayType::Scalar& Val3) noexcept {
            ArrayType& Me = static_cast<ArrayType&>(*this);
            Me[0] = Val0;
            Me[1] = Val1;
            Me[2] = Val2;
            Me[3] = Val3;
        }
    };
}