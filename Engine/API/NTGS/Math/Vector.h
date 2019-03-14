#pragma once

#include <NTGS/Math/VectorTraits.h>

namespace NTGS {
    template<typename T, int DIM>
    class Vector : public VectorTraits<Vector<T, DIM>, T, DIM> {
    public:
        using ScalarType = T;
        static constexpr int DIMENSION = DIM;

        ScalarType mData[DIM] = { ScalarType(0) };

        inline Vector() = default;
        inline explicit Vector(const ScalarType& cValue) noexcept { SetData<1>(cValue); }
        inline explicit Vector(const ScalarType* pData) noexcept { SetData<DIM>(pData); }
        inline Vector(const std::initializer_list<ScalarType>& cList) { assert(cList.size() >= DIM); SetData<DIM>(cList.begin()); }
        template<typename U, int DIM2> 
        inline Vector(const Vector<U, DIM2>& cVec) noexcept { SetData<DIM2>(cVec.mData);}
        template<typename U, int DIM2>
        inline Vector& operator=(const Vector<U, DIM2>& cVec) { SetData<DIM2>(cVec.mData); return *this; }
    
        inline ScalarType& operator[](int i) noexcept { return mData[i]; }
        inline const ScalarType& operator[](int i) const noexcept { return mData[i]; }
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
