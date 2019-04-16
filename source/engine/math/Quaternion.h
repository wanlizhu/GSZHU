#pragma once

#include "math/Vector.h"
#include "math/Matrix4.h"
#include "math/QuaternionOperators.h"

namespace NTGS {
    template<typename T>
    struct Quaternion : public QuaternionOperators<Quaternion, T> {
        using Element = T;

        union {
            Array<T, 1, 4> mArray;
            struct { T x, y, z, w; };
            struct { Vector<T, 3> v; T w; };
        };

        Quaternion()
            : x(0), y(0), z(0), w(1) 
        {}
        Quaternion(T x, T y, T z, T w)
            : x(x), y(y), z(z), w(w) 
        {}
        Quaternion(const Vector<T, 3>& vec, T w)
            : vec(vec), w(w) 
        {}
        Quaternion(const std::initializer_list<T>& list) {
            assert(list.size() >= 4);
            auto iter = list.begin();
            for (int i = 0; i < 4; i++, iter++)
                mArray[i] = *iter;
        }
        template<typename U>
        Quaternion(const Quaternion<U>& other) 
            : v(other.v), w(other.w)
        {}

        inline T& operator[](int i) {
            assert((uint32_t)i < 4);
            return mArray[i];
        }

        inline const T& operator[](int i) const {
            assert((uint32_t) i < 4);
            return mArray[i];
        }

        template<typename U>
        inline Vector<U, 3> operator()(const Vector<U, 3>& vec) const {
            
        }

        static Quaternion Create(T angle, const Vector<T, 3>& axis) {
        
        }
    };
}