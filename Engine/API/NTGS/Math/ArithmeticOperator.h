#pragma once

#include <stdint.h>
#include <math.h>
#include <numeric>
#include <functional>

namespace NTGS {
    template<typename ArrayType>
    class ArithmeticOperator {
    public:
        // -
        ArrayType operator-() const {
            const ArrayType& Me = static_cast<const ArrayType&>(*this);
            ArrayType New;
            ArrayType::IndexType Index;
            while (Index) {
                New[Index] = -Arr[Index];
                Index++;
            }
            return Me;
        }

        // + and -
        inline ArrayType& operator+=(const ArrayType& Arr) noexcept {
            ArrayType& Me = static_cast<ArrayType&>(*this);
            ArrayType::IndexType Index;
            while (Index) {
                Me[Index] += Arr[Index];
                Index++;
            }
            return Me;
        }

        inline ArrayType& operator-=(const ArrayType& Arr) noexcept {
            ArrayType& Me = static_cast<ArrayType&>(*this);
            ArrayType::IndexType Index;
            while (Index) {
                Me[Index] -= Arr[Index];
                Index++;
            }
            return Me;
        }

        friend inline ArrayType operator+(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return Arr1 += Arr2;
        }

        friend inline ArrayType operator-(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return Arr1 -= Arr2;
        }

        // * and /
        inline ArrayType operator*=(const ArrayType& Arr) noexcept {
            ArrayType& Me = static_cast<ArrayType&>(*this);
            ArrayType::IndexType Index;
            while (Index) {
                Me[Index] *= Arr[Index];
                Index++;
            }
            return Me;
        }

        inline ArrayType operator/=(const ArrayType& Arr) noexcept {
            ArrayType& Me = static_cast<ArrayType&>(*this);
            ArrayType::IndexType Index;
            while (Index) {
                Me[Index] /= Arr[Index];
                Index++;
            }
            return Me;
        }

        friend inline ArrayType operator*(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return Arr1 *= Arr2;
        }

        friend inline ArrayType operator/(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return Arr1 /= Arr2;
        }

        friend inline ArrayType operator*(const ArrayType& Arr, const ArrayType::Scalar& Factor) noexcept {
            return Arr * ArrayType(Factor);
        }

        friend inline ArrayType operator/(const ArrayType& Arr, const ArrayType::Scalar& Factor) noexcept {
            return Arr / ArrayType(Factor);
        }

        friend inline ArrayType operator*(const ArrayType::Scalar& Factor, const ArrayType& Arr) noexcept {
            return ArrayType(Factor) * Arr;
        }

        friend inline ArrayType operator/(const ArrayType::Scalar& Factor, const ArrayType& Arr) noexcept {
            return ArrayType(Factor) / Arr;
        }
    };
}