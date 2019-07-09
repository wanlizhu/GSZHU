#pragma once
#include <ZHUENGINE/Core/BasicTypes.h>

namespace ZHU
{
    template<typename T>
    class TVector;

    template<typename T>
    class TPoint : public Eigen::Matrix<T, 3, 1>
    {
    public:
        using BASE = Eigen::Matrix<T, 3, 1>;
        using VECTOR = typename TVector<T>;

        TPoint() : BASE() {}
        TPoint(const BASE& other) : BASE(other) {}
        TPoint(const Vec4f& other) : BASE(other[0], other[1], other[2]) {}
        TPoint(const VECTOR& vec) : BASE(vec[0], vec[1], vec[2]) {}
        TPoint(const T& n) : BASE(n, n, n) {}
        TPoint(const T& _x, const T& _y, const T& _z) : BASE(_x, _y, _z) {}

        inline operator Vec4f() const { return Vec4f((*this)[0], (*this)[1], (*this)[2], 1.f); }
        inline float& x() noexcept { return (*this)[0]; }
        inline float& y() noexcept { return (*this)[1]; }
        inline float& z() noexcept { return (*this)[2]; }
        inline const float& x() const noexcept { return (*this)[0]; }
        inline const float& y() const noexcept { return (*this)[1]; }
        inline const float& z() const noexcept { return (*this)[2]; }
    };

    typedef TPoint<float> Point;

    template<typename T>
    class TVector : public Eigen::Matrix<T, 3, 1>
    {
    public:
        using BASE = Eigen::Matrix<T, 3, 1>;
        using POINT = typename TPoint<T>;

        TVector() : BASE() {}
        TVector(const BASE& other) : BASE(other) {}
        TVector(const Vec4f& other) : BASE(other[0], other[1], other[2]) {}
        TVector(const POINT& point) : BASE(point[0], point[1], point[2]) {}
        TVector(const T& n) : BASE(n, n, n) {}
        TVector(const T& _x, const T& _y, const T& _z) : BASE(_x, _y, _z) {}

        inline operator Vec4f() const { return Vec4f((*this)[0], (*this)[1], (*this)[2], 0.f); }
        inline float& x() noexcept { return (*this)[0]; }
        inline float& y() noexcept { return (*this)[1]; }
        inline float& z() noexcept { return (*this)[2]; }
        inline const float& x() const noexcept { return (*this)[0]; }
        inline const float& y() const noexcept { return (*this)[1]; }
        inline const float& z() const noexcept { return (*this)[2]; }
    };

    typedef TVector<float> Vector;
}