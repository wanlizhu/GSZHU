#pragma once

#include "Details/VectorTraits.h"

namespace NTGS {

    template<typename T, int Dim>
    struct NTGS_API Vector : public ArrayTraits<Vector<T, Dim>, T, Dim> {
        T mData[Dim] = { T(0) };

        Vector() {};
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
            auto iter = list.begin();
            for (int i = 0; i < Dim; i++) {
                mData[i] = static_cast<T>(*(iter + i));
            }
        }
        template<typename V, typename U, int Dim2,
                 typename = std::enable_if<Dim2 >= Dim>>
        Vector(const ArrayAccess<V, U, Dim2>& array) {
            for (int i = 0; i < Dim; i++) {
                mData[i] = static_cast<T>(array[i]);
            }
        }
    };




    template<typename T>
    struct NTGS_API Vector<T, 2> : public ArrayTraits<Vector<T, 2>, T, 2> {
        union {
            T mData[2] = { T(0) };
            struct { T x, y; };
            struct { T u, v; };
        };

        Vector() {};
        template<typename U>
        explicit Vector(const U& value)
            : x(static_cast<T>(value))
            , y(static_cast<T>(value))
        {}
        template<typename U>
        Vector(const U& val0, const U& val1)
            : x(static_cast<T>(val0))
            , y(static_cast<T>(val1))
        {}
        template<typename U>
        Vector(const std::initializer_list<U>& list) {
            assert(list.size() >= 2);
            const U* iter = list.begin();
            x = static_cast<T>(*iter);
            y = static_cast<T>(*(++iter));
        }
        template<typename U, int Dim,
                 typename = std::enable_if<Dim >= 2>>
        Vector(const Vector<U, Dim>& other)
            : x(static_cast<T>(other[0]))
            , y(static_cast<T>(other[1]))
        {}
    };




    template<typename T>
    struct NTGS_API Vector<T, 3> : public ArrayTraits<Vector<T, 3>, T, 3> {
        union {
            T mData[3] = { T(0) };
            struct { T x, y, z; };
            struct { T r, g, b; };
        };

        Vector() {};
        template<typename U>
        explicit Vector(const U& value)
            : x(static_cast<T>(value))
            , y(static_cast<T>(value))
            , z(static_cast<T>(value))
        {}
        template<typename U>
        Vector(const U& val0, const U& val1, const U& val2)
            : x(static_cast<T>(val0))
            , y(static_cast<T>(val1))
            , z(static_cast<T>(val2))
        {}
        template<typename U>
        Vector(const std::initializer_list<U>& list) {
            assert(list.size() >= 3);
            auto iter = list.begin();
            x = static_cast<T>(*iter);
            y = static_cast<T>(*(++iter));
            z = static_cast<T>(*(++iter));
        }
        template<typename U, int Dim,
                 typename = std::enable_if<Dim >= 3>>
        Vector(const Vector<U, Dim>& other)
            : x(static_cast<T>(other[0]))
            , y(static_cast<T>(other[1]))
            , z(static_cast<T>(other[2]))
        {}
        template<typename U1, typename U2>
        Vector(const Vector<U1, 2>& v, const U2& num)
            : x(static_cast<T>(v.x))
            , y(static_cast<T>(v.y))
            , z(static_cast<T>(num))
        {}
        template<typename U1, typename U2>
        Vector(const U1& num, const Vector<U2, 2>& v)
            : x(static_cast<T>(num))
            , y(static_cast<T>(v.x))
            , z(static_cast<T>(v.y))
        {}
    };




    template<typename T>
    struct NTGS_API Vector<T, 4> : public ArrayTraits<Vector<T, 4>, T, 4> {
        union {
            T mData[4] = { T(0) };
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };
        };

        Vector() {};
        template<typename U>
        explicit Vector(const U& value)
            : x(static_cast<T>(value))
            , y(static_cast<T>(value))
            , z(static_cast<T>(value))
            , w(static_cast<T>(value))
        {}
        template<typename U>
        Vector(const U& val0, const U& val1, const U& val2, const U& val3)
            : x(static_cast<T>(val0))
            , y(static_cast<T>(val1))
            , z(static_cast<T>(val2))
            , w(static_cast<T>(val3))
        {}
        template<typename U>
        Vector(const std::initializer_list<U>& list) {
            assert(list.size() >= 4);
            auto iter = list.begin();
            x = static_cast<T>(*iter);
            y = static_cast<T>(*(++iter));
            z = static_cast<T>(*(++iter));
            w = static_cast<T>(*(++iter));
        }
        template<typename U, int Dim,
                 typename = std::enable_if<Dim >= 4>>
        Vector(const Vector<U, Dim>& other)
            : x(static_cast<T>(other[0]))
            , y(static_cast<T>(other[1]))
            , z(static_cast<T>(other[2]))
            , w(static_cast<T>(other[3]))
        {}
        template<typename U1, typename U2, typename U3>
        Vector(const Vector<U1, 2>& v, const U2& num1, const U3& num2)
            : x(static_cast<T>(v.x))
            , y(static_cast<T>(v.y))
            , z(static_cast<T>(num1))
            , w(static_cast<T>(num2))
        {}
        template<typename U1, typename U2>
        Vector(const Vector<U1, 3>& v, const U2& num)
            : x(static_cast<T>(v.x))
            , y(static_cast<T>(v.y))
            , z(static_cast<T>(v.z))
            , w(static_cast<T>(num))
        {}
    };




    template<typename T, int Rows, int Cols>
    struct NTGS_API Matrix : public Vector<Vector<T, Rows>, Cols> {
        using BaseType = Vector<Vector<T, Rows>, Cols>;
        using ColumnType = Vector<T, Rows>;
        using RowType = Vector<T, Cols>;
        using ScalarType = T;
        using TransposeType = Vector<Vector<T, Cols>, Rows>;
        static constexpr int COLUMNS = Cols;
        static constexpr int ROWS = Rows;

        Matrix() {};
        template<typename U>
        explicit Matrix(const Vector<U, Rows>& value) : BaseType(value) {}
        template<typename U, int Rows2, int Cols2>
        Matrix(const Matrix<U, Rows2, Cols2>& other) : BaseType(other) {}
        template<typename U>
        Matrix(const std::initializer_list<Vector<U, Rows>>& list) : BaseType(list) {}

        template<typename U, int Dim,
                 typename = std::enable_if<Dim >= (Rows*Cols)>>
        Matrix(const Vector<U, Dim>& vec) {
            for (int i = 0; i < Cols; i++) {
                for (int j = 0; j < Rows; j++) {
                    mData[i][j] = static_cast<T>(vec[Rows * i + j]);
                }
            }
        }
    };




    template<typename T>
    struct NTGS_API Quaternion : public ArrayComparisonOperators<Quaternion<T>, T, 4> {
        using ElementType = T;

        union {
            T mData[4] = { 0, 0, 0, 1 };
            struct { T x, y, z, w; };
            struct { Vector<T, 3> v; T w; };
        };

        Quaternion() = default;
        template<typename U>
        Quaternion(const U& x, const U& y, const U& z, const U& w)
            : x(static_cast<T>(x))
            , y(static_cast<T>(y))
            , z(static_cast<T>(z))
            , w(static_cast<T>(w))
        {}
        template<typename U1, typename U2>
        Quaternion(const Vector<U1, 3>& v, const U2& w) 
            : v(Vector<T, 3>(v))
            , w(static_cast<U2>(w))
        {}
        template<typename U>
        Quaternion(const Quaternion<U>& other) 
            : v(Vector<T, 3>(other.v))
            , w(static_cast<T>(other.w))
        {}
        template<typename U>
        Quaternion(const std::initializer_list<U>& list) {
            assert(list.size() >= 4);
            const auto& iter = list.begin();
            x = static_cast<T>(*iter);
            y = static_cast<T>(*(++iter));
            z = static_cast<T>(*(++iter));
            w = static_cast<T>(*(++iter));
        }
        template<typename U, int Dim,
                 typename = std::enable_if<Dim >= 4>>
            Quaternion(const Vector<U, Dim>& vec)
            : x(static_cast<T>(vec[0]))
            , y(static_cast<T>(vec[1]))
            , z(static_cast<T>(vec[2]))
            , w(static_cast<T>(vec[3]))
        {}
    };




    typedef Vector<int, 2> Vec2i;
    typedef Vector<float, 2> Vec2f;
    typedef Vector<double, 2> Vec2d;
    typedef Vector<int, 3> Vec3i;
    typedef Vector<float, 3> Vec3f;
    typedef Vector<double, 3> Vec3d;
    typedef Vector<int, 4> Vec4i;
    typedef Vector<float, 4> Vec4f;
    typedef Vector<double, 4> Vec4d;

    typedef Matrix<int, 2, 2> Mat2i;
    typedef Matrix<float, 2, 2> Mat2f;
    typedef Matrix<double, 2, 2> Mat2d;
    typedef Matrix<int, 3, 3> Mat3i;
    typedef Matrix<float, 3, 3> Mat3f;
    typedef Matrix<double, 3, 3> Mat3d;
    typedef Matrix<int, 4, 4> Mat4i;
    typedef Matrix<float, 4, 4> Mat4f;
    typedef Matrix<double, 4, 4> Mat4d;

    typedef Quaternion<float> Quatf;
    typedef Quaternion<double> Quatd;
}