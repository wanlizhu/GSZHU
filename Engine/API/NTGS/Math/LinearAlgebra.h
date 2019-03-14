#pragma once

#include <NTGS/Math/Vector.h>
#include <NTGS/Math/Matrix.h>

namespace NTGS {
    template<typename T, int DIM>
    inline typename Vector<T, DIM>::ScalarType Dot(const Vector<T, DIM>& cV1, const Vector<T, DIM>& cV2) noexcept {
        typename Vector<T, DIM>::ScalarType cSum(0);
        for (int i = 0; i < DIM; i++) {
            cSum += cV1[i] * cV2[i];
        }
        return cSum;
    }

    template<typename T>
    inline Vector<T, 3> Cross(const Vector<T, 3>& cV1, const Vector<T, 3>& cV2) noexcept {
        return Vector<T, 3>(
            cV1.GetY() * cV2.GetZ() - cV2.GetY() * cV1.GetZ(),
            cV1.GetZ() * cV2.GetX() - cV2.GetZ() * cV1.GetX(),
            cV1.GetX() * cV2.GetY() - cV2.GetX() * cV1.GetY());
    }

    template<typename T, int DIM>
    inline typename Vector<T, DIM>::ScalarType operator*(const Vector<T, DIM>& cV1, const Vector<T, DIM>& cV2) noexcept {
        return Dot(cV1, cV2);
    }

    template<typename T>
    inline Vector<T, 3> operator^(const Vector<T, 3>& cV1, const Vector<T, 3>& cV2) noexcept {
        return Cross(cV1, cV2);
    }

    template<typename T, int DIM>
    inline typename Vector<T, DIM>::ScalarType Length2(const Vector<T, DIM>& cV) noexcept {
        typename Vector<T, DIM>::ScalarType cSum(0);
        for (int i = 0; i < DIM; i++) {
            cSum += cV[i] * cV[i];
        }
        return cSum;
    }

    template<typename T, int DIM>
    inline typename Vector<T, DIM>::ScalarType Length(const Vector<T, DIM>& cV) noexcept {
        return std::sqrt(Length2(cV));
    }

    template<typename T, int DIM>
    inline typename Vector<T, DIM>::ScalarType Distance(const Vector<T, DIM>& cV1, const Vector<T, DIM>& cV2) noexcept {
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
        static_assert(_MAJOR1 == _MINOR2, "");
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