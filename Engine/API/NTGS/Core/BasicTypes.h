#pragma once

#include "Details/VectorTraits.h"

namespace NTGS {
    template<typename T, int Dim>
    struct NTGS_API Vector : public ArrayTraits<Vector<T, Dim>, T, Dim> {
        T mData[Dim] = { T(0) };

        Vector() = default;
        template<typename U>
        explicit Vector(const U& value) {
            Set(value);
        }
        template<typename U, int Dim2>
        Vector(const Vector<U, Dim2>& other) {
            static_assert(Dim <= Dim2, "");
            for (int i = 0; i < Dim; i++) {
                mData[i] = static_cast<T>(other.mData[i]);
            }
        }
        template<typename U>
        Vector(const std::initializer_list<U>& list) {
            assert(list.size() >= Dim);
            for (int i = 0; i < Dim; i++) {
                mData[i] = static_cast<T>(*(list.begin() + i));
            }
        }
    };


    template<typename T>
    struct NTGS_API Vector2 : public ArrayTraits<Vector2<T>, T, 2> {
        union {
            T mData[2] = { T(0) };
            struct { T x, y; };
            struct { T u, v; };
        };

        Vector2() = default;
        template<typename U>
        explicit Vector2(const U& value)
            : x(static_cast<T>(value))
            , y(static_cast<T>(value))
        {}
        template<typename U>
        Vector2(const U& val0, const U& val1)
            : x(static_cast<T>(val0))
            , y(static_cast<T>(val1))
        {}
        template<typename U>
        Vector2(const Vector2<U>& other)
            : x(static_cast<T>(other.x))
            , y(static_cast<T>(other.y))
        {}
        template<typename U>
        Vector2(const std::initializer_list<U>& list) {
            assert(list.size() >= 2);
            x = static_cast<T>(*list.begin());
            y = static_cast<T>(*(++list.begin()));
        }
        template<typename U, int Dim>
        Vector2(const Vector<U, Dim>& other)
            : x(static_cast<T>(other[0]))
            , y(static_cast<T>(other[1]))
        {}

        template<typename U>
        operator const Vector<U, 2>() const {
            return reinterpret_cast<const Vector<U, 2>>(*this);
        }
        template<typename U>
        operator Vector<U, 2>() {
            return reinterpret_cast<const Vector<U, 2>>(*this);
        }
    };


    template<typename T>
    struct NTGS_API Vector3 : public ArrayTraits<Vector3<T>, T, 3> {
        union {
            T mData[3] = { T(0) };
            struct { T x, y, z; };
            struct { T r, g, b; };
        };

        Vector3() = default;
        template<typename U>
        explicit Vector3(const U& value)
            : x(static_cast<T>(value))
            , y(static_cast<T>(value))
            , z(static_cast<T>(value))
        {}
        template<typename U>
        Vector3(const U& val0, const U& val1, const U& val2)
            : x(static_cast<T>(val0))
            , y(static_cast<T>(val1))
            , z(static_cast<T>(val2))
        {}
        template<typename U>
        Vector3(const Vector3<U>& other)
            : x(static_cast<T>(other.x))
            , y(static_cast<T>(other.y))
            , z(static_cast<T>(other.z))
        {}
        template<typename U>
        Vector3(const std::initializer_list<U>& list) {
            assert(list.size() >= 3);
            const auto& iter = list.begin();
            x = static_cast<T>(*iter);
            y = static_cast<T>(*(++iter));
            z = static_cast<T>(*(++iter));
        }
        template<typename U, int Dim>
        Vector3(const Vector<U, Dim>& other)
            : x(static_cast<T>(other[0]))
            , y(static_cast<T>(other[1]))
            , z(static_cast<T>(other[2]))
        {}
        template<typename U1, typename U2>
        Vector3(const Vector2<U1>& v, const U2& num)
            : x(static_cast<T>(v.x))
            , y(static_cast<T>(v.y))
            , z(static_cast<T>(num))
        {}
        template<typename U1, typename U2>
        Vector3(const U1& num, const Vector2<U2>& v)
            : x(static_cast<T>(num))
            , y(static_cast<T>(v.x))
            , z(static_cast<T>(v.y))
        {}

        template<typename U>
        operator const Vector<U, 3>() const {
            return reinterpret_cast<const Vector<U, 3>>(*this);
        }
        template<typename U>
        operator Vector<U, 3>() {
            return reinterpret_cast<const Vector<U, 3>>(*this);
        }
    };


    template<typename T>
    struct NTGS_API Vector4 : public ArrayTraits<Vector4<T>, T, 4> {
        union {
            T mData[4] = { T(0) };
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };
        };

        Vector4() = default;
        template<typename U>
        explicit Vector4(const U& value)
            : x(static_cast<T>(value))
            , y(static_cast<T>(value))
            , z(static_cast<T>(value))
            , w(static_cast<T>(value))
        {}
        template<typename U>
        Vector4(const U& val0, const U& val1, const U& val2, const U& val3)
            : x(static_cast<T>(val0))
            , y(static_cast<T>(val1))
            , z(static_cast<T>(val2))
            , w(static_cast<T>(val3))
        {}
        template<typename U>
        Vector4(const Vector4<U>& other)
            : x(static_cast<T>(other.x))
            , y(static_cast<T>(other.y))
            , z(static_cast<T>(other.z))
            , w(static_cast<T>(other.w))
        {}
        template<typename U>
        Vector4(const std::initializer_list<U>& list) {
            assert(list.size() >= 4);
            const auto& iter = list.begin();
            x = static_cast<T>(*iter);
            y = static_cast<T>(*(++iter));
            z = static_cast<T>(*(++iter));
            w = static_cast<T>(*(++iter));
        }
        template<typename U, int Dim>
        Vector4(const Vector<U, Dim>& other)
            : x(static_cast<T>(other[0]))
            , y(static_cast<T>(other[1]))
            , z(static_cast<T>(other[2]))
            , w(static_cast<T>(other[3]))
        {}
        template<typename U1, typename U2, typename U3>
        Vector4(const Vector2<U1>& v, const U2& num1, const U3& num2)
            : x(static_cast<T>(v.x))
            , y(static_cast<T>(v.y))
            , z(static_cast<T>(num1))
            , w(static_cast<T>(num2))
        {}
        template<typename U1, typename U2>
        Vector4(const Vector3<U1>& v, const U2& num)
            : x(static_cast<T>(v.x))
            , y(static_cast<T>(v.y))
            , z(static_cast<T>(v.z))
            , w(static_cast<T>(num))
        {}

        template<typename U>
        operator const Vector<U, 4>() const {
            return reinterpret_cast<const Vector<U, 4>>(*this);
        }
        template<typename U>
        operator Vector<U, 4>() {
            return reinterpret_cast<const Vector<U, 4>>(*this);
        }
    };


    template<typename T, int Rows, int Cols>
    struct Matrix : public Vector<Vector<T, Rows>, Cols> {
        using BaseType = Vector<Vector<T, Rows>, Cols>;
        using ColumnType = Vector<T, Rows>;
        using TransposeType = Vector<Vector<T, Cols>, Rows>;
        static constexpr int COLUMNS = Cols;
        static constexpr int ROWS = Rows;

        Matrix() { SetIdentity(); };
        template<typename UColType>
        explicit Matrix(const UColType& value) : BaseType(value) {}
        template<typename U, int Rows2, int Cols2>
        Matrix(const Matrix<U, Rows2, Cols2>& other) : BaseType(other) {}
        template<typename UColType>
        Matrix(const std::initializer_list<UColType>& list) : BaseType(list) {}

        inline bool IsIdentity() const {
            if (Rows != Cols)
                return false;
            for (int i = 0; i < Cols; i++) {
                for (int j = 0; j < Rows; j++) {
                    if (i == j) {
                        if (std::abs(mData[i][j] - T(1)) > std::numeric_limits<T>::epsilon())
                            return false;
                    }
                    else {
                        if (std::abs(mData[i][j] - T(0)) > std::numeric_limits<T>::epsilon())
                            return false;
                    }
                }
            }
            return true;
        }

        inline void SetIdentity() {
            assert(Rows == Cols);
            Set(ColumnType(0));
            for (int i = 0; i < Cols; i++) {
                mData[i][i] = T(1);
            }
        }
    };


    typedef Vector2<int> Vec2i;
    typedef Vector2<float> Vec2f;
    typedef Vector2<double> Vec2d;
    typedef Vector3<int> Vec3i;
    typedef Vector3<float> Vec3f;
    typedef Vector3<double> Vec3d;
    typedef Vector4<int> Vec4i;
    typedef Vector4<float> Vec4f;
    typedef Vector4<double> Vec4d;

    typedef Matrix<int, 2, 2> Mat2i;
    typedef Matrix<float, 2, 2> Mat2f;
    typedef Matrix<double, 2, 2> Mat2d;
    typedef Matrix<int, 3, 3> Mat3i;
    typedef Matrix<float, 3, 3> Mat3f;
    typedef Matrix<double, 3, 3> Mat3d;
    typedef Matrix<int, 4, 4> Mat4i;
    typedef Matrix<float, 4, 4> Mat4f;
    typedef Matrix<double, 4, 4> Mat4d;
}