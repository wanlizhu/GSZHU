#pragma once

#include <NTGS/Math/Array.h>

namespace NTGS {
    template<typename T, int _DIMENSION>
    class ALIGN16 Vector : public Array<T, _DIMENSION> {
    public:
        typedef Array<T, _DIMENSION> ArrayType;
        static constexpr int DIMENSION = _DIMENSION;

        Vector() : ArrayType() {}
        Vector(const Scalar& Value) : ArrayType(Value) {}
        Vector(const Scalar* Data) : ArrayType(Data) {}
        Vector(const Scalar& Val0, const Scalar& Val1) : ArrayType(Val0, Val1) { static_assert(DIMENSION >= 2, ""); }
        Vector(const Scalar& Val0, const Scalar& Val1, const Scalar& Val2) : ArrayType(Val0, Val1, Val2) { static_assert(DIMENSION >= 3, ""); }
        Vector(const Scalar& Val0, const Scalar& Val1, const Scalar& Val2, const Scalar& Val3) : ArrayType(Val0, Val1, Val2, Val3) { static_assert(DIMENSION >= 4, ""); }
        Vector(const std::initializer_list<Scalar>& List) : ArrayType(List) {}
        template<typename U, int DIM>
        Vector(const Vector<U, DIM>& Arr) 
            : ArrayType(static_cast<const Array<U, DIM>&>(Arr))
        {}

        inline Scalar& GetU() noexcept { return mData[0]; }
        inline Scalar& GetV() noexcept { static_assert(DIMENSION >= 2, ""); return mData[1]; }
        inline Scalar& GetR() noexcept { return mData[0]; }
        inline Scalar& GetG() noexcept { static_assert(DIMENSION >= 2, ""); return mData[1]; }
        inline Scalar& GetB() noexcept { static_assert(DIMENSION >= 3, ""); return mData[2]; }
        inline Scalar& GetA() noexcept { static_assert(DIMENSION >= 4, ""); return mData[3]; }
        inline Scalar& GetX() noexcept { return mData[0]; }
        inline Scalar& GetY() noexcept { static_assert(DIMENSION >= 2, ""); return mData[1]; }
        inline Scalar& GetZ() noexcept { static_assert(DIMENSION >= 3, ""); return mData[2]; }
        inline Scalar& GetW() noexcept { static_assert(DIMENSION >= 4, ""); return mData[3]; }

        inline const Scalar& GetU() const noexcept { return mData[0]; }
        inline const Scalar& GetV() const noexcept { static_assert(DIMENSION >= 2, ""); return mData[1]; }
        inline const Scalar& GetR() const noexcept { return mData[0]; }
        inline const Scalar& GetG() const noexcept { static_assert(DIMENSION >= 2, ""); return mData[1]; }
        inline const Scalar& GetB() const noexcept { static_assert(DIMENSION >= 3, ""); return mData[2]; }
        inline const Scalar& GetA() const noexcept { static_assert(DIMENSION >= 4, ""); return mData[3]; }
        inline const Scalar& GetX() const noexcept { return mData[0]; }
        inline const Scalar& GetY() const noexcept { static_assert(DIMENSION >= 2, ""); return mData[1]; }
        inline const Scalar& GetZ() const noexcept { static_assert(DIMENSION >= 3, ""); return mData[2]; }
        inline const Scalar& GetW() const noexcept { static_assert(DIMENSION >= 4, ""); return mData[3]; }
    };

    typedef Vector<int, 2> Vec2i;
    typedef Vector<int, 3> Vec3i;
    typedef Vector<int, 4> Vec4i;
    typedef Vector<float, 2> Vec2;
    typedef Vector<float, 3> Vec3;
    typedef Vector<float, 4> Vec4;
    typedef Vector<double, 2> Vec2d;
    typedef Vector<double, 3> Vec3d;
    typedef Vector<double, 4> Vec4d;
}
