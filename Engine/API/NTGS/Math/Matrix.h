#pragma once

#include <NTGS/Math/Vector.h>

namespace NTGS {

    template<typename T, int _MAJOR, int _MINOR>
    class ALIGN16 Matrix : public Array<Array<T, _MINOR>, _MAJOR> {
    public:
        using ArrayType = Array<Array<T, _MINOR>, _MAJOR>;
        using VectorType = Vector<T, _MINOR>;
        using ScalarType = typename Array<T, _MINOR>::ScalarType;
        static constexpr int MAJOR = _MAJOR;
        static constexpr int MINOR = _MINOR;
        static constexpr int LENGTH = _MAJOR * _MINOR;

        inline Matrix() = default;
        inline explicit Matrix(const VectorType& Value) : ArrayType(Value) {}
        inline explicit Matrix(const VectorType* Data) : ArrayType(Data) {}
        inline explicit Matrix(const ArrayType& Arr) : ArrayType(Arr) {}
        inline Matrix(const Matrix& Mat) : ArrayType(Mat.GetArray()) {}
        inline Matrix(Matrix&& Mat) noexcept : ArrayType(std::move(Mat.GetArray())) {}
        inline Matrix(const std::initializer_list<VectorType>& List) : ArrayType(List) {}
        template<typename U, int _MAJOR2, int _MINOR2> inline Matrix(const Matrix<U, _MAJOR2, _MINOR2>& Mat) {
            static_assert((MAJOR <= _MAJOR2) && (MINOR <= _MINOR2), "");
            for (int i = 0; i < MAJOR; i++) {
                (*this)[i] = Mat[i];
            }
        }
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