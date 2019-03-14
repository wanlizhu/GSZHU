#pragma once

#include <NTGS/Math/VectorTraits.h>

namespace NTGS {
    template<typename T, int _DIMENSION>
    class Vector : public VectorTraits<Vector<T, _DIMENSION>, T, _DIMENSION> {
    public:
        using ScalarType = T;
        static constexpr int DIMENSION = _DIMENSION;
        static constexpr int LENGTH = _DIMENSION;

        ScalarType mData[DIMENSION] = { (ScalarType)0 };

        inline Vector() = default;
        inline explicit Vector(const ScalarType& cValue) noexcept { Fill(cValue); }
        inline explicit Vector(const ScalarType* pData) noexcept { Copy(pData); }
        inline Vector(const ScalarType& cVal0, const ScalarType& cVal1) { Set(cVal0, cVal1); }
        inline Vector(const ScalarType& cVal0, const ScalarType& cVal1, const ScalarType& cVal2) { Set(cVal0, cVal1, cVal2); }
        inline Vector(const ScalarType& cVal0, const ScalarType& cVal1, const ScalarType& cVal2, const ScalarType& cVal3) { Set(cVal0, cVal1, cVal2, cVal3); }
        inline Vector(const std::initializer_list<ScalarType>& cList) { assert(cList.size() >= DIMENSION); Copy(cList.begin()); }
        template<typename U, int _DIMENSION2> 
        inline Vector(const Vector<U, _DIMENSION2>& cVec) { 
            static_assert(DIMENSION <= _DIMENSION2, ""); 
            Copy(cVec.mData);
        }
        template<typename U, int _DIMENSION2>
        inline Vector& operator=(const Vector<U, _DIMENSION2>& cVec) {
            static_assert(DIMENSION <= _DIMENSION2, "");
            Copy(cVec.mData);
            return *this;
        }
    
        inline ScalarType& operator[](int i) noexcept { return mData[i]; }
        inline const ScalarType& operator[](int i) const noexcept { return mData[i]; }
        inline operator ScalarType*() noexcept { return mData; }
        inline operator const ScalarType*() const noexcept { return mData; }
        inline ScalarType& GetX() noexcept { return mData[0]; }
        inline ScalarType& GetY() noexcept { static_assert(DIMENSION >= 2, ""); return mData[1]; }
        inline ScalarType& GetZ() noexcept { static_assert(DIMENSION >= 3, ""); return mData[2]; }
        inline ScalarType& GetW() noexcept { static_assert(DIMENSION >= 4, ""); return mData[3]; }
        inline const ScalarType& GetX() const noexcept { return mData[0]; }
        inline const ScalarType& GetY() const noexcept { static_assert(DIMENSION >= 2, ""); return mData[1]; }
        inline const ScalarType& GetZ() const noexcept { static_assert(DIMENSION >= 3, ""); return mData[2]; }
        inline const ScalarType& GetW() const noexcept { static_assert(DIMENSION >= 4, ""); return mData[3]; }
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
