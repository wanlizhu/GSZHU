#pragma once

#include <NTGS/Math/Vector.h>
#include <NTGS/Math/Matrix.h>

namespace NTGS {
    template<typename T, int DIM>
    inline typename Vector<T, DIM>::Scalar Dot(const Vector<T, DIM>& cV1, const Vector<T, DIM>& cV2) noexcept {
        typename Vector<T, DIM>::Scalar cSum(0);
        for (int i = 0; i < DIM; i++) {
            cSum += cV1[i] * cV2[i];
        }
        return cSum;
    }

    template<typename T>
    inline Vector<T, 3> Cross(const Vector<T, 3>& cV1, const Vector<T, 3>& cV2) noexcept {
        return Vector<T, 3>(
            cV1.Y * cV2.Z - cV2.Y * cV1.Z,
            cV1.Z * cV2.X - cV2.Z * cV1.X,
            cV1.X * cV2.Y - cV2.X * cV1.Y);
    }

    template<typename T, int DIM>
    inline typename Vector<T, DIM>::Scalar Length2(const Vector<T, DIM>& cV) noexcept {
        typename Vector<T, DIM>::Scalar cSum(0);
        for (int i = 0; i < DIM; i++) {
            cSum += cV[i] * cV[i];
        }
        return cSum;
    }

    template<typename T, int DIM>
    inline typename Vector<T, DIM>::Scalar Length(const Vector<T, DIM>& cV) noexcept {
        return std::sqrt(Length2(cV));
    }

    template<typename T, int DIM>
    inline typename Vector<T, DIM>::Scalar Distance(const Vector<T, DIM>& cV1, const Vector<T, DIM>& cV2) noexcept {
        return Length(cV2 - cV1);
    }

    template<typename T, int DIM>
    inline Vector<T, DIM> Normalize(const Vector<T, DIM>& cV) noexcept {
        return Vector<T, DIM>(cV) /= Length(cV);
    }


    template<typename T, int _MAJOR1, int _MINOR1, typename U, int _MAJOR2, int _MINOR2>
    inline void Mul(const Matrix<T, _MAJOR1, _MINOR1>& cMat1,
                    const Matrix<U, _MAJOR2, _MINOR2>& cMat2,
                    Matrix<T, _MAJOR2, _MINOR1>* pProduct) noexcept {
        static_assert(_MAJOR1 == _MINOR2);
        static const int _INNER = _MAJOR1;

        std::memset(pProduct, 0, sizeof(Matrix<T, _MAJOR2, _MINOR1>));

        for (int Major = 0; Major < _MAJOR2; Major++) {
            for (int Minor = 0; Minor < _MINOR1; Minor++) {
                for (int Inner = 0; Inner < _INNER; Inner++) {
                    (*pProduct)[Major][Minor] += cMat1[Major][Inner] * cMat2[Inner][Minor];
                }
            }
        }
    }

    template<typename T, int _MAJOR1, int _MINOR1, typename U, int _MAJOR2, int _MINOR2>
    inline Matrix<T, _MAJOR2, _MINOR1> Mul(const Matrix<T, _MAJOR1, _MINOR1>& cMat1,
                                           const Matrix<U, _MAJOR2, _MINOR2>& cMat2) noexcept {
        Matrix<T, _MAJOR2, _MINOR1> cProduct;
        Mul(cMat1, cMat2, &cProduct);
        return cProduct;
    }                        

    template<typename T, int _MAJOR1, int _MINOR1, typename U, int _MAJOR2, int _MINOR2>
    inline Matrix<T, _MAJOR2, _MINOR1> operator*(const Matrix<T, _MAJOR1, _MINOR1>& cMat1,
                                                 const Matrix<U, _MAJOR2, _MINOR2>& cMat2) noexcept {
        Matrix<T, _MAJOR2, _MINOR1> cProduct;
        Mul(cMat1, cMat2, &cProduct);
        return cProduct;
    }
    
}