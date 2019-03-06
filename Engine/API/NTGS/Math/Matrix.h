#pragma once

#include <NTGS/Math/Vector.h>

namespace NTGS {
    template<typename T, int _ROWS, int _COLS>
    class Matrix : public Array<T, _ROWS, _COLS, true> {
    public:
        typedef Array<T, _ROWS, _COLS, true> ArrayType;
        static constexpr int ROWS = _ROWS;
        static constexpr int COLS = _COLS;


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

    template<typename T, int DIM>
    inline Matrix<T, DIM, DIM> Mul(const Matrix<T, DIM, DIM>& Mat1, const Matrix<T, DIM, DIM>& Mat2) noexcept {
        M
    }

    template<typename T, int DIM>
    inline Vector<T, DIM> Mul(const Matrix<T, DIM, DIM>& Mat, const Vector<T, DIM>& Vec) noexcept {
        
    }
}