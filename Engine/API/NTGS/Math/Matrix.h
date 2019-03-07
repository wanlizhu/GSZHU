#pragma once

#include <NTGS/Math/Vector.h>

namespace NTGS {
    template<typename T, int _ROWS, int _COLS>
    class Matrix : public Array<T, _ROWS, _COLS, true> {
    public:
        typedef Array<T, _ROWS, _COLS, true> ArrayType;
        typedef Vector<T, _ROWS> VectorType;
        static constexpr int ROWS = _ROWS;
        static constexpr int COLS = _COLS;

        Matrix() = default;
        Matrix(const Scalar& Value) : ArrayType(Value) {}
        Matrix(const Scalar* Data) : ArrayType(Data) {}
        Matrix(const std::initializer_list<Scalar>& List) : ArrayType(List) {}

        template<typename U, int U_ROWS, int U_COLS>
        Matrix(const Matrix<U, U_ROWS, U_COLS>& Mat) {
            static_assert(ROWS <= U_ROWS && COLS <= U_COLS);
            IndexType Index;
            while (Index) {
                (*this)[Index] = (T)Arr[Index];
                Index++;
            }
        }

        VectorType& Column(int Col) { return static_cast<VectorType&>((*this)[IndexType(0, Col)]); }
        const VectorType& Column(int Col) const { return static_cast<VectorType&>((*this)[IndexType(0, Col)]); }
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