#pragma once

#include <NTGS/Math/ArrayOperators.h>

namespace NTGS {
    template<typename T, int _DIMENSIONS>
    class ALIGN16 Vector : public ArrayOperators<Vector<T, _DIMENSIONS>> {
    public:
        using Scalar = T;
        static constexpr int DIMENSIONS = _DIMENSIONS;
        static constexpr Vector ZERO;
        static constexpr Vector ONE;
        static constexpr Vector UP;
        static constexpr Vector FORWARD;
        static constexpr Vector RIGHT;

        ALIGN16 union {
            Scalar mData[DIMENSIONS] = { (Scalar)0 };
            struct { Scalar X, Y, Z; };
        };

        inline Vector() = default;
        inline explicit Vector(const Scalar& Value) noexcept { Fill(Value); }
        inline explicit Vector(const Scalar* Data) noexcept { Copy(Data); }
        inline Vector(const Scalar& Val0, const Scalar& Val1) { Set(Val0, Val1); }
        inline Vector(const Scalar& Val0, const Scalar& Val1, const Scalar& Val2) { Set(Val0, Val1, Val2); }
        inline Vector(const Scalar& Val0, const Scalar& Val1, const Scalar& Val2, const Scalar& Val3) { Set(Val0, Val1, Val2, Val3); }
        inline Vector(const std::initializer_list<Scalar>& List) { assert(List.size() >= DIMENSIONS); Copy(List.begin()); }
        template<typename U, int _DIMENSIONS2> 
        inline Vector(const Vector<U, _DIMENSIONS2>& Vec) { 
            static_assert(DIMENSIONS < _DIMENSIONS2, ""); 
            Copy(static_cast<const Scalar*>(Vec.mData));
        }
    
        inline Scalar& operator[](int i) noexcept { return mData[i]; }
        inline const Scalar& operator[](int i) const noexcept { return mData[i]; }
        inline operator Scalar*() noexcept { return mData; }
        inline operator const Scalar*() const noexcept { return mData; }
    };

    typedef Vector<int, 2> Vector2i;
    typedef Vector<int, 3> Vector3i;
    typedef Vector<int, 4> Vector4i;
    typedef Vector<float, 2> Vector2;
    typedef Vector<float, 3> Vector3;
    typedef Vector<float, 4> Vector4;
    typedef Vector<double, 2> Vector2d;
    typedef Vector<double, 3> Vector3d;
    typedef Vector<double, 4> Vector4d;
}
