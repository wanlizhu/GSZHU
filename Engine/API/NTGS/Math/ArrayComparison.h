#pragma once

#include <math.h>
#include <numeric>
#include <limits>
#include <NTGS/Core/Config.h>

namespace NTGS {

    template<typename T>
    bool IsEquivalent(const T& A, const T& B) noexcept {
        if (std::is_floating_point<T>::value)
            return std::abs(A - B) <= std::numeric_limits<T>::epsilon();
        else
            return A == B;
    }

    template<typename ArrayType, typename T, int DIMENSION>
    class ArrayComparison {
    public:
        friend inline bool operator==(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            for (int i = 0; i < DIMENSION; i++) {
                if (!IsEquivalent(Arr1[i], Arr2[i]))
                    return false;
            }
            return true;
        }

        friend inline bool operator!=(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return !(Arr1 == Arr2);
        }

        friend inline bool operator<(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            for (int i = 0; i < DIMENSION; i++) {
                if (!(Arr1[i] < Arr2[i]))
                    return false;
            }
            return true;
        }

        friend inline bool operator>=(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return !(Arr1 < Arr2);
        }

        friend inline bool operator>(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            for (int i = 0; i < DIMENSION; i++) {
                if (!(Arr1[i] > Arr2[i]))
                    return false;
            }
            return true;
        }

        friend inline bool operator<=(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return !(Arr1 > Arr2);
        }
    };
}