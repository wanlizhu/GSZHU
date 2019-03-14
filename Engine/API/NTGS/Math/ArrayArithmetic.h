#pragma once

#include <stdint.h>
#include <math.h>
#include <numeric>
#include <functional>
#include <NTGS/Math/ArrayComparison.h>

namespace NTGS {
    template<typename ArrayType, typename T, int DIMENSION>
    class ArrayArithmetic : public ArrayComparison<ArrayType, T, DIMENSION> {
    public:
        // -
        inline ArrayType operator-() const noexcept {
            const ArrayType& cMe = static_cast<const ArrayType&>(*this);
            ArrayType cNew;
            for (int i = 0; i < DIMENSION; i++) {
                cNew[i] = -cMe[i];
            }
            return cNew;
        }

        // + and -
        inline ArrayType& operator+=(const ArrayType& cArr) noexcept {
            ArrayType& cMe = static_cast<ArrayType&>(*this);
            for (int i = 0; i < DIMENSION; i++) {
                cMe[i] += cArr[i];
            }
            return cMe;
        }

        inline ArrayType& operator-=(const ArrayType& cArr) noexcept {
            ArrayType& cMe = static_cast<ArrayType&>(*this);
            for (int i = 0; i < DIMENSION; i++) {
                cMe[i] -= cArr[i];
            }
            return cMe;
        }

        friend inline ArrayType operator+(const ArrayType& cArr1, const ArrayType& cArr2) noexcept {
            return ArrayType(cArr1) += cArr2;
        }

        friend inline ArrayType operator-(const ArrayType& cArr1, const ArrayType& cArr2) noexcept {
            return ArrayType(cArr1) -= cArr2;
        }

        // * and /
        inline ArrayType& operator*=(const T& Factor) noexcept {
            ArrayType& cMe = static_cast<ArrayType&>(*this);
            for (int i = 0; i < DIMENSION; i++) {
                cMe[i] *= Factor;
            }
            return cMe;
        }

        inline ArrayType& operator/=(const T& Factor) noexcept {
            ArrayType& cMe = static_cast<ArrayType&>(*this);
            for (int i = 0; i < DIMENSION; i++) {
                cMe[i] /= Factor;
            }
            return cMe;
        }

        friend inline ArrayType operator*(const ArrayType& cArr, const T& Factor) noexcept {
            ArrayType cNew(cArr);
            for (int i = 0; i < DIMENSION; i++) {
                cNew[i] *= Factor;
            }
            return cNew;
        }

        friend inline ArrayType operator/(const ArrayType& cArr, const T& Factor) noexcept {
            ArrayType cNew(cArr);
            for (int i = 0; i < DIMENSION; i++) {
                cNew[i] /= Factor;
            }
            return cNew;
        }

        friend inline ArrayType operator*(const T& Factor, const ArrayType& cArr) noexcept {
            ArrayType cNew(cArr);
            for (int i = 0; i < DIMENSION; i++) {
                cNew[i] = Factor * cNew[i];
            }
            return cNew;
        }

        friend inline ArrayType operator/(const T& Factor, const ArrayType& cArr) noexcept {
            ArrayType cNew(cArr);
            for (int i = 0; i < DIMENSION; i++) {
                cNew[i] = Factor / cNew[i];
            }
            return cNew;
        }
    };
}