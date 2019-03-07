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

    template<template<typename T, int LEN> class _ArrayType, typename Scalar, int LEN>
    class ComparisonOperator {
    public:
        typedef _ArrayType<Scalar, LEN> ArrayType;

        friend inline bool operator==(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            for (int i = 0; i < ArrayType::LENGTH; i++) {
                if (!IsEquivalent(Arr1[i], Arr2[i]))
                    return false;
            }
            return true;
        }

        friend inline bool operator!=(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return !(Arr1 == Arr2);
        }

        friend inline bool operator<(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            for (int i = 0; i < ArrayType::LENGTH; i++) {
                if (!(Arr1[i] < Arr2[i]))
                    return false;
            }
            return true;
        }

        friend inline bool operator>=(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return !(Arr1 < Arr2);
        }

        friend inline bool operator>(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            for (int i = 0; i < ArrayType::LENGTH; i++) {
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