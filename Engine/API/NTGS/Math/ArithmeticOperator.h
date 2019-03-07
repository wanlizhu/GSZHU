#pragma once

#include <stdint.h>
#include <math.h>
#include <numeric>
#include <functional>

namespace NTGS {
    template<template<typename T, int LEN> class _ArrayType, typename Scalar, int LEN>
    class ArithmeticOperator {
    public:
        typedef _ArrayType<Scalar, LEN> ArrayType;

        // -
        ArrayType operator-() const {
            const ArrayType& Me = static_cast<const ArrayType&>(*this);
            ArrayType New;
            for (int i = 0; i < ArrayType::LENGTH; i++) {
                New[i] = -Me[i];
            }
            return New;
        }

        // + and -
        inline ArrayType& operator+=(const ArrayType& Arr) noexcept {
            ArrayType& Me = static_cast<ArrayType&>(*this);
            for (int i = 0; i < ArrayType::LENGTH; i++) {
                Me[i] += Arr[i];
            }
            return Me;
        }

        inline ArrayType& operator-=(const ArrayType& Arr) noexcept {
            ArrayType& Me = static_cast<ArrayType&>(*this);
            for (int i = 0; i < ArrayType::LENGTH; i++) {
                Me[i] -= Arr[i];
            }
            return Me;
        }

        friend inline ArrayType operator+(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return ArrayType(Arr1) += Arr2;
        }

        friend inline ArrayType operator-(const ArrayType& Arr1, const ArrayType& Arr2) noexcept {
            return ArrayType(Arr1) -= Arr2;
        }

        // * and /
        friend inline ArrayType operator*(const ArrayType& Arr, const Scalar& Factor) noexcept {
            ArrayType New(Arr);
            for (int i = 0; i < ArrayType::LENGTH; i++) {
                New[i] *= Factor;
            }
            return New;
        }

        friend inline ArrayType operator/(const ArrayType& Arr, const Scalar& Factor) noexcept {
            ArrayType New(Arr);
            for (int i = 0; i < ArrayType::LENGTH; i++) {
                New[i] /= Factor;
            }
            return New;
        }

        friend inline ArrayType operator*(const Scalar& Factor, const ArrayType& Arr) noexcept {
            ArrayType New(Arr);
            for (int i = 0; i < ArrayType::LENGTH; i++) {
                New[i] = Factor * New[i];
            }
            return New;
        }

        friend inline ArrayType operator/(const Scalar& Factor, const ArrayType& Arr) noexcept {
            ArrayType New(Arr);
            for (int i = 0; i < ArrayType::LENGTH; i++) {
                New[i] = Factor / New[i];
            }
            return New;
        }
    };
}