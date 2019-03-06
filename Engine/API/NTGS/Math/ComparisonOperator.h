#pragma once

#include <math.h>
#include <numeric>
#include <limits>
#include <NTGS/Core/Config.h>

namespace NTGS {

    template<typename T>
    bool IsEquivalent(const T& A, const T& B) noexcept {
        return A == B;
    }

    template<>
    bool IsEquivalent<float>(const float& A, const float& B) noexcept {
#ifdef EPSILON
        return std::abs(A - B) <= EPSILON;
#else
        return std::abs(A - B) <= std::numeric_limits<float>::epsilon();
#endif
    }

    template<>
    bool IsEquivalent<double>(const double& A, const double& B) noexcept {
        return std::abs(A - B) <= std::numeric_limits<double>::epsilon();
    }

    template<typename ArrayType>
    class ComparisonOperator {
    public:
        friend inline bool operator==(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            ArrayType::IndexType Index;
            while (Index) {
                if (!IsEquivalent(Arr1[Index], Arr2[Index]))
                    return false;
                Index++;
            }
            return true;
        }

        friend inline bool operator!=(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return !(Arr1 == Arr2);
        }

        friend inline bool operator<(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            ArrayType::IndexType Index;
            while (Index) {
                if (!(Arr1[Index] < Arr2[Index]))
                    return false;
                Index++;
            }
            return true;
        }

        friend inline bool operator>=(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return !(Arr1 < Arr2);
        }

        friend inline bool operator>(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            ArrayType::IndexType Index;
            while (Index) {
                if (!(Arr1[Index] > Arr2[Index]))
                    return false;
                Index++;
            }
            return true;
        }

        friend inline bool operator<=(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return !(Arr1 > Arr2);
        }

        inline bool HasNaN() const noexcept {
            ArrayType::IndexType Index;
            const ArrayType& Me = static_cast<const ArrayType&>(*this);
            while (Index) {
                if (std::isnan(Me[Index]))
                    return true;
                Index++;
            }
            return false;
        }
    };
}