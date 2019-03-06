#pragma once

#include <NTGS/Math/Array.h>

namespace NTGS {
    template<typename T, int _DIMENSION>
    class Vector : public Array<T, _DIMENSION, 1, true> {
    public:
        typedef Array<T, _DIMENSION, 1, true> ArrayType;
        static constexpr int DIMENSION = _DIMENSION;

        Vector() : ArrayType() {}
        Vector(const Scalar& Value) : ArrayType(Value) {}
        Vector(const Scalar* Data) : ArrayType(Data) {}
        Vector(const Scalar& Val0, const Scalar& Val1) : ArrayType(Val0, Val1) { static_assert(DIMENSION >= 2); }
        Vector(const Scalar& Val0, const Scalar& Val1, const Scalar& Val2) : ArrayType(Val0, Val1, Val2) { static_assert(DIMENSION >= 3); }
        Vector(const Scalar& Val0, const Scalar& Val1, const Scalar& Val2, const Scalar& Val3) : ArrayType(Val0, Val1, Val2, Val3) { static_assert(DIMENSION >= 4); }
        Vector(const std::initializer_list<Scalar>& List) : ArrayType(List) {}

        template<typename U, int DIM>
        Vector(const Vector<U, DIM>& Arr) {
            static_assert(DIMENSION <= DIM);
            Vector<T, DIMENSION>::IndexType Index;
            while (Index) {
                (*this)[Index] = (T) Arr[Index];
            }
        }

        inline Scalar& GetU() noexcept { return mData[0]; }
        inline Scalar& GetV() noexcept { static_assert(DIMENSION >= 2); return mData[1]; }
        inline Scalar& GetR() noexcept { return mData[0]; }
        inline Scalar& GetG() noexcept { static_assert(DIMENSION >= 2); return mData[1]; }
        inline Scalar& GetB() noexcept { static_assert(DIMENSION >= 3); return mData[2]; }
        inline Scalar& GetA() noexcept { static_assert(DIMENSION >= 4); return mData[3]; }
        inline Scalar& GetX() noexcept { return mData[0]; }
        inline Scalar& GetY() noexcept { static_assert(DIMENSION >= 2); return mData[1]; }
        inline Scalar& GetZ() noexcept { static_assert(DIMENSION >= 3); return mData[2]; }
        inline Scalar& GetW() noexcept { static_assert(DIMENSION >= 4); return mData[3]; }

        inline const Scalar& GetU() const noexcept { return mData[0]; }
        inline const Scalar& GetV() const noexcept { static_assert(DIMENSION >= 2); return mData[1]; }
        inline const Scalar& GetR() const noexcept { return mData[0]; }
        inline const Scalar& GetG() const noexcept { static_assert(DIMENSION >= 2); return mData[1]; }
        inline const Scalar& GetB() const noexcept { static_assert(DIMENSION >= 3); return mData[2]; }
        inline const Scalar& GetA() const noexcept { static_assert(DIMENSION >= 4); return mData[3]; }
        inline const Scalar& GetX() const noexcept { return mData[0]; }
        inline const Scalar& GetY() const noexcept { static_assert(DIMENSION >= 2); return mData[1]; }
        inline const Scalar& GetZ() const noexcept { static_assert(DIMENSION >= 3); return mData[2]; }
        inline const Scalar& GetW() const noexcept { static_assert(DIMENSION >= 4); return mData[3]; }
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


    template<typename T, int DIM>
    inline Vector<T, DIM>::Scalar Dot(const Vector<T, DIM>& V1, const Vector<T, DIM>& V2) noexcept {
        Vector<T, DIM>::Scalar Sum(0);
        Vector<T, DIM>::IndexType Index;
        while (Index) {
            Sum += V1[Index] * V2[Index];
            Index++;
        }
        return Sum;
    }

    template<typename T>
    inline Vector<T, 3> Cross(const Vector<T, 3>& V1, const Vector<T, 3>& V2) noexcept {
        return Vector<T, 3>(
            V1.GetY() * V2.GetZ() - V2.GetY() * V1.GetZ(),
            V1.GetZ() * V2.GetX() - V2.GetZ() * V1.GetX(),
            V1.GetX() * V2.GetY() - V2.GetX() * V1.GetY());
    }

    template<typename T, int DIM>
    inline inline Vector<T, DIM> Length2(const Vector<T, DIM>& V) noexcept {
        Vector<T, DIM>::Scalar Sum(0);
        Vector<T, DIM>::IndexType Index;
        while (Index) {
            Sum += V[Index] * V[Index];
            Index++;
        }
        return Sum;
    }

    template<typename T, int DIM>
    inline inline Vector<T, DIM> Length(const Vector<T, DIM>& V) noexcept {
        return std::sqrt(Length2(V));
    }

    template<typename T, int DIM>
    inline inline Vector<T, DIM> Normalize(const Vector<T, DIM>& V) noexcept {
        return Vector<T, DIM>(V) /= Length(V);
    }
}
