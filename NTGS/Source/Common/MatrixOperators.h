#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdint.h>
#include <memory>
#include <stdio.h>
#include <type_traits>
#include "Common/Config.h"
#include "Common/Array.h"

namespace NTGS {
    template<template<typename T, int Cols, int Rows> class _MAT, typename T, int Cols, int Rows>
    struct MatrixOperators {
    private:
        using MAT = _MAT<T, Cols, Rows>;
        using INDEX = typename Array<T, Cols, Rows>::Index;

    public:
        // Operator +
        template<typename U>
        inline MAT& operator+=(const _MAT<U, Cols, Rows>& mat) {
            auto& cThis = static_cast<MAT&>(*this);
            for (int i = 0; i < Cols; i++)
                for (int j = 0; j < Rows; j++)
                    cThis.mArray[INDEX(i, j)] += mat.mArray[INDEX(i, j)];
            return cThis;
        }
        
        template<typename U>
        inline MAT operator+(const _MAT<U, Cols, Rows>& mat) const {
            return MAT(static_cast<const MAT&>(*this)) += mat;
        }

        // Operator -
        inline MAT operator-() const {
            MAT ret(static_cast<const MAT&>(*this));
            for (int i = 0; i < Cols; i++)
                for (int j = 0; j < Rows; j++)
                    ret.mArray[INDEX(i, j)] = -ret.mArray[INDEX(i, j)];
            return ret;
        }

        template<typename U>
        inline MAT& operator-=(const _MAT<U, Cols, Rows>& mat) {
            auto& cThis = static_cast<MAT&>(*this);
            for (int i = 0; i < Cols; i++)
                for (int j = 0; j < Rows; j++)
                    cThis.mArray[INDEX(i, j)] -= mat.mArray[INDEX(i, j)];
            return cThis;
        }

        template<typename U>
        inline MAT operator-(const _MAT<U, Cols, Rows>& mat) const {
            return MAT(static_cast<const MAT&>(*this)) -= mat;
        }

        // Operator *
        template<typename U,
                 typename = typename std::enable_if<std::is_arithmetic<U>::value>::type>
        inline MAT& operator*=(const U& num) {
            auto& cThis = static_cast<MAT&>(*this);
            for (int i = 0; i < Cols; i++)
                for (int j = 0; j < Rows; j++)
                    cThis.mArray[INDEX(i, j)] *= num;
            return cThis;
        }

        template<typename U,
                 typename = typename std::enable_if<std::is_arithmetic<U>::value>::type>
        inline MAT operator*(const U& num) {
            return MAT(static_cast<const MAT&>(*this)) *= num;
        }

        template<typename U,
                 typename = typename std::enable_if<std::is_arithmetic<U>::value>::type>
        friend inline MAT operator*(const U& num, const MAT& mat) {
            return MAT(mat) *= num;
        }

        // Operator /
        template<typename U,
                 typename = typename std::enable_if<std::is_arithmetic<U>::value>::type>
        inline MAT& operator/=(const U& num) {
            auto& cThis = static_cast<MAT&>(*this);
            for (int i = 0; i < Cols; i++)
                for (int j = 0; j < Rows; j++)
                    cThis.mArray[INDEX(i, j)] /= num;
            return cThis;
        }

        template<typename U,
                 typename = typename std::enable_if<std::is_arithmetic<U>::value>::type>
        inline MAT operator/(const U& num) {
            return MAT(static_cast<const MAT&>(*this)) /= num;
        }

        template<typename U,
                 typename = typename std::enable_if<std::is_arithmetic<U>::value>::type>
        friend inline MAT operator/(const U& num, const MAT& mat) {
            MAT ret(mat);
            for (int i = 0; i < Cols; i++)
                for (int j = 0; j < Rows; j++)
                    ret.mArray[INDEX(i, j)] = num / ret.mArray[INDEX(i, j)];
            return ret;
        }

        // Comparison
        friend inline bool operator==(const MAT& mat1, const MAT& mat2) {
            for (int i = 0; i < Cols; i++)
                for (int j = 0; j < Rows; j++) {
                    if (std::is_floating_point<T>::value) {
                        if (!FLOAT_EQ(mat1.mArray[INDEX(i, j)], mat2.mArray[INDEX(i, j)]))
                            return false;
                    }
                    else {
                        if (!(mat1.mArray[INDEX(i, j)] == mat2.mArray[INDEX(i, j)]))
                            return false;
                    }
                }
            return true;
        }

        friend inline bool operator!=(const MAT& mat1, const MAT& mat2) {
            return !(mat1 == mat2);
        }
    };
}
