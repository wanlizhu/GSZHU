#pragma once

#include <NTGS/Math/Vector.h>

namespace NTGS {

    template<typename T, int _MAJOR, int _MINOR>
    class Matrix;

    template<typename T, int _MAJOR1, int _MINOR1, typename U, int _MAJOR2, int _MINOR2>
    void Mul(const Matrix<T, _MAJOR1, _MINOR1>& Mat1,
             const Matrix<U, _MAJOR2, _MINOR2>& Mat2,
             Matrix<T, _MAJOR2, _MINOR1>* Product) noexcept;

    template<typename T, int _MAJOR, int _MINOR>
    class ALIGN16 Matrix : public Array<Vector<T, _MINOR>, _MAJOR> {
    public:
        typedef Array<Vector<T, _MINOR>, _MAJOR> ArrayType;
        typedef Vector<T, _MINOR> VectorType;
        typedef T Scalar;
        static constexpr int MAJOR = _MAJOR;
        static constexpr int MINOR = _MINOR;
        static constexpr int LENGTH = _MAJOR * _MINOR;
        static constexpr int BYTES = _MAJOR * _MINOR * sizeof(T);

        Matrix() = default;
        Matrix(const VectorType& Value) : ArrayType(Value) {}
        Matrix(const VectorType* Data) : ArrayType(Data) {}
        Matrix(const std::initializer_list<VectorType>& List) : ArrayType(List) {}
        template<typename U, int U_MAJOR, int U_MINOR>
        Matrix(const Matrix<U, U_MAJOR, U_MINOR>& Mat) 
            : ArrayType(static_cast<const Array<Vector<U, U_MAJOR>, U_MINOR>&>(Mat))
        {}

        template<typename U, int U_MAJOR, int U_MINOR>
        inline Matrix& operator*=(const Matrix<U, U_MAJOR, U_MINOR>& Mat) {
            Matrix<T, U_MAJOR, MINOR> Product;
            Mul<T, MAJOR, MINOR, U, U_MAJOR, U_MINOR>(*this, Mat, &Product);
            return Product;
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