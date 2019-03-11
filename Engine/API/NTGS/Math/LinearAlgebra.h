#pragma once

#include <NTGS/Math/Array.h>
#include <NTGS/Math/Matrix.h>

namespace NTGS {
    template<typename T, int DIM>
    inline typename Vector<T, DIM>::Scalar Dot(const Vector<T, DIM>& V1, const Vector<T, DIM>& V2) noexcept {
        typename Vector<T, DIM>::Scalar Sum(0);
        for (int i = 0; i < DIM; i++) {
            Sum += V1[i] * V2[i];
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
    inline typename Vector<T, DIM>::Scalar Length2(const Vector<T, DIM>& V) noexcept {
        typename Vector<T, DIM>::Scalar Sum(0);
        for (int i = 0; i < DIM; i++) {
            Sum += V[i] * V[i];
        }
        return Sum;
    }

    template<typename T, int DIM>
    inline typename Vector<T, DIM>::Scalar Length(const Vector<T, DIM>& V) noexcept {
        return std::sqrt(Length2(V));
    }

    template<typename T, int DIM>
    inline typename Vector<T, DIM>::Scalar Distance(const Vector<T, DIM>& V1, const Vector<T, DIM>& V2) noexcept {
        return Length(V2 - V1);
    }

    template<typename T, int DIM>
    inline Vector<T, DIM> Normalize(const Vector<T, DIM>& V) noexcept {
        return Vector<T, DIM>(V) /= Length(V);
    }



     /*
    template<typename T, int _MAJOR1, int _MINOR1, typename U, int _MAJOR2, int _MINOR2>
    inline void Mul(const Matrix<T, _MAJOR1, _MINOR1>& Mat1,
                    const Matrix<U, _MAJOR2, _MINOR2>& Mat2,
                    Matrix<T, _MAJOR2, _MINOR1>* Product) noexcept {
        static_assert(_MAJOR1 == _MINOR2);
        static const int _INNER = _MAJOR1;

        std::memset(Product, 0, sizeof(Matrix<T, _MAJOR2, _MINOR1>));

        for (int Major = 0; Major < _MAJOR2; Major++) {
            for (int Minor = 0; Minor < _MINOR1; Minor++) {
                for (int Inner = 0; Inner < _INNER; Inner++) {
                    (*Product)[Major][Minor] += Mat1[Major][Inner] * Mat2[Inner][Minor];
                }
            }
        }
    }

    template<typename T, int _MAJOR1, int _MINOR1, typename U, int _MAJOR2, int _MINOR2>
    inline Matrix<T, _MAJOR2, _MINOR1> Mul(const Matrix<T, _MAJOR1, _MINOR1>& Mat1,
                                           const Matrix<U, _MAJOR2, _MINOR2>& Mat2) noexcept {
        Matrix<T, _MAJOR2, _MINOR1> Product;
        Mul(Mat1, Mat2, &Product);
        return Product;
    }                        

    template<typename T, int _MAJOR1, int _MINOR1, typename U, int _MAJOR2, int _MINOR2>
    inline Matrix<T, _MAJOR2, _MINOR1> operator*(const Matrix<T, _MAJOR1, _MINOR1>& Mat1,
                                                 const Matrix<U, _MAJOR2, _MINOR2>& Mat2) noexcept {
        Matrix<T, _MAJOR2, _MINOR1> Product;
        Mul(Mat1, Mat2, &Product);
        return Product;
    }

    template<typename T, int _MAJOR, int _MINOR, typename U>
    inline Vector<T, _MINOR> Mul(const Matrix<T, _MAJOR, _MINOR>& Mat, const Vector<U, _MINOR>& Vec) noexcept {
        return static_cast<Vector<T, _MINOR>>(Mul(Mat, static_cast<const Matrix<U, 1, _MINOR>&>(Vec)));
    }
    */
}