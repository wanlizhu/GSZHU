#pragma once

#include <NTGS/Math/Vector.h>
#include <NTGS/Math/MatrixTraits.h>

namespace NTGS {

    template<typename T, int _MAJOR, int _MINOR>
    class Matrix : public MatrixTraits<Matrix<T, _MAJOR, _MINOR>, T, _MAJOR, _MINOR> {
    public:
        using VectorType = Vector<T, _MINOR>;
        using ScalarType = T;
        static constexpr int MAJOR = _MAJOR;
        static constexpr int MINOR = _MINOR;
        static constexpr int LENGTH = _MAJOR * _MINOR;

        VectorType mData[MAJOR];

        inline Matrix() = default;
        inline explicit Matrix(const VectorType& cValue) { Fill(cValue); }
        inline explicit Matrix(const VectorType* pData) { Copy(pData); }
        inline Matrix(const std::initializer_list<VectorType>& cList) { assert(cList.size() >= LENGTH); Copy(cList.begin()); }
        template<typename U, int _MAJOR2, int _MINOR2> 
        inline Matrix(const Matrix<U, _MAJOR2, _MINOR2>& cMat) {
            static_assert((MAJOR <= _MAJOR2) && (MINOR <= _MINOR2), "");
            for (int i = 0; i < MAJOR; i++) {
                mData[i] = cMat[i];
            }
        }
        template<typename U, int _MAJOR2, int _MINOR2>
        inline Matrix& operator=(const Matrix<U, _MAJOR2, _MINOR2>& cMat) {
            static_assert((MAJOR <= _MAJOR2) && (MINOR <= _MINOR2), "");
            for (int i = 0; i < MAJOR; i++) {
                mData[i] = cMat[i];
            }
            return *this;
        }

        inline typename VectorType& operator[](int i) noexcept { return mData[i]; }
        inline const typename VectorType& operator[](int i) const noexcept { return mData[i]; }
        inline operator typename VectorType*() noexcept { return mData; }
        inline operator const typename VectorType*() const noexcept { return mData; }
    };

    typedef Matrix<int, 2, 2> Matrix2i;
    typedef Matrix<int, 3, 3> Matrix3i;
    typedef Matrix<int, 4, 4> Matrix4i;
    typedef Matrix<float, 2, 2> Matrix2;
    typedef Matrix<float, 3, 3> Matrix3;
    typedef Matrix<float, 4, 4> Matrix4;
    typedef Matrix<double, 2, 2> Matrix2d;
    typedef Matrix<double, 3, 3> Matrix3d;
    typedef Matrix<double, 4, 4> Matrix4d;   
}      