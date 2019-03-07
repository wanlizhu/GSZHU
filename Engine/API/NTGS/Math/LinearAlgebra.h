#pragma once

#include <NTGS/Math/Array.h>
#include <NTGS/Math/Matrix.h>

namespace NTGS {
    template<typename T, int DIM>
    inline Vector<T, DIM>::Scalar Dot(const Vector<T, DIM>& V1, const Vector<T, DIM>& V2) noexcept {
        Vector<T, DIM>::Scalar Sum(0);
        Vector<T, DIM>::IndexType Index;
        while (Index) {
            Sum += V1[Index] * V2[Index];
            Index++;
        }
        return Sum;
    }

    template<typename T>
    inline Vector<T, 3> Cross(const Vector<T, 3>& V1, const Vector<T, 3>& V2) noexcept {
        return Vector<T, 3>(
            V1.GetY() * V2.GetZ() - V2.GetY() * V1.GetZ(),
            V1.GetZ() * V2.GetX() - V2.GetZ() * V1.GetX(),
            V1.GetX() * V2.GetY() - V2.GetX() * V1.GetY());
    }

    template<typename T, int DIM>
    inline inline Vector<T, DIM> Length2(const Vector<T, DIM>& V) noexcept {
        Vector<T, DIM>::Scalar Sum(0);
        Vector<T, DIM>::IndexType Index;
        while (Index) {
            Sum += V[Index] * V[Index];
            Index++;
        }
        return Sum;
    }

    template<typename T, int DIM>
    inline inline Vector<T, DIM> Length(const Vector<T, DIM>& V) noexcept {
        return std::sqrt(Length2(V));
    }

    template<typename T, int DIM>
    inline inline Vector<T, DIM> Normalize(const Vector<T, DIM>& V) noexcept {
        return Vector<T, DIM>(V) /= Length(V);
    }

    template<typename T, int DIM>
    inline Matrix<T, DIM, DIM> Mul(const Matrix<T, DIM, DIM>& Mat1, const Matrix<T, DIM, DIM>& Mat2) noexcept {

    }

    template<typename T, int DIM>
    inline Vector<T, DIM> Mul(const Matrix<T, DIM, DIM>& Mat, const Vector<T, DIM>& Vec) noexcept {

    }
}