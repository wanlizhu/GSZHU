#pragma once

#include <NTGS/Math/Vector.h>
#include <NTGS/Math/MatrixOperators.h>

namespace NTGS {

    template<typename T, int _MAJOR, int _MINOR>
    class ALIGN16 Matrix : public MatrixOperators<Matrix<T, _MAJOR, _MINOR>> {
    public:
        using Vector = Vector<T, _MINOR>;
        using Scalar = T;
        static constexpr int MAJOR = _MAJOR;
        static constexpr int MINOR = _MINOR;
        static constexpr int LENGTH = _MAJOR * _MINOR;

        Matrix::Vector mData[MAJOR];

        inline Matrix() = default;
        inline explicit Matrix(const Matrix::Vector& Value) { Fill(Value); }
        inline explicit Matrix(const Matrix::Vector* Data) : { Copy(Data); }
        inline Matrix(const std::initializer_list<Matrix::Vector>& List) { Copy(List.begin()); }
        template<typename U, int _MAJOR2, int _MINOR2> 
        inline Matrix(const Matrix<U, _MAJOR2, _MINOR2>& Mat) {
            static_assert((MAJOR <= _MAJOR2) && (MINOR <= _MINOR2), "");
            for (int i = 0; i < MAJOR; i++) {
                mData[i] = Mat[i];
            }
        }

        inline typename Matrix::Vector& operator[](int i) noexcept { return mData[i]; }
        inline const typename Matrix::Vector& operator[](int i) const noexcept { return mData[i]; }
        inline operator typename Matrix::Vector*() noexcept { return mData; }
        inline operator const typename Matrix::Vector*() const noexcept { return mData; }
    };

    typedef Matrix<int, 2, 2> Mat2i;
    typedef Matrix<int, 3, 3> Mat3i;
    typedef Matrix<int, 4, 4> Mat4i;
    typedef Matrix<float, 2, 2> Mat2;
    typedef Matrix<float, 3, 3> Mat3;
    typedef Matrix<float, 4, 4> Mat4;
    typedef Matrix<double, 2, 2> Mat2d;
    typedef Matrix<double, 3, 3> Mat3d;
    typedef Matrix<double, 4, 4> Mat4d;   
}      