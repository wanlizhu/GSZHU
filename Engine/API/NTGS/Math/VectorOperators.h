#pragma once

#include <NTGS/Core/Config.h>
#include <NTGS/Math/ArrayArithmetic.h>
#include <NTGS/Math/ArrayComparison.h>
#include <assert.h>
#include <memory>
#include <type_traits>

namespace NTGS {

    template<template<typename T, int DIM> class _VectorType, typename T, int DIM>
    class VectorOperators : public ArrayArithmetic<_VectorType, T, DIM>
                          , public ArrayComparison<_VectorType, T, DIM> {
    private:
        using VectorType = _VectorType<T, DIM>;

    protected:
        inline void Fill(const typename VectorType::Scalar& Value) noexcept {
            VectorType& Me = static_cast<VectorType&>(*this);
            for (int i = 0; i < VectorType::LENGTH; i++) {
                Me.mData[i] = Value;
            }
        }

        inline void Copy(const typename VectorType::Scalar* Data) noexcept {
            VectorType& Me = static_cast<VectorType&>(*this);
            for (int i = 0; i < VectorType::LENGTH; i++) {
                Me.mData[i] = Data[i];
            }
        }

        template<typename = std::enable_if<VectorType::LENGTH >= 2>>
        inline void Set(const typename VectorType::Scalar& Val0, const typename VectorType::Scalar& Val1) noexcept {
            VectorType& Me = static_cast<VectorType&>(*this);
            Me[0] = Val0;
            Me[1] = Val1;
        }

        template<typename = std::enable_if<VectorType::LENGTH >= 3>>
        inline void Set(const typename VectorType::Scalar& Val0, 
                        const typename VectorType::Scalar& Val1,
                        const typename VectorType::Scalar& Val2) noexcept {
            VectorType& Me = static_cast<VectorType&>(*this);
            Me[0] = Val0;
            Me[1] = Val1;
            Me[2] = Val2;
        }

        template<typename = std::enable_if<VectorType::LENGTH >= 4>>
        inline void Set(const typename VectorType::Scalar& Val0,
                        const typename VectorType::Scalar& Val1,
                        const typename VectorType::Scalar& Val2,
                        const typename VectorType::Scalar& Val3) noexcept {
            VectorType& Me = static_cast<VectorType&>(*this);
            Me[0] = Val0;
            Me[1] = Val1;
            Me[2] = Val2;
            Me[3] = Val3;
        }
    };
}