#pragma once

#include "Common/Config.h"
#include <numeric>
#include <limits>

namespace ZHU
{
#define FLOAT_EQ(num1, num2)  (std::abs(num1 - num2) < std::numeric_limits<float>::epsilon())
#define FLOAT_EPS_EQ(num1, num2, eps)  (std::abs(num1 - num2) < eps)

    template<typename T>
    using BitSet = typename std::underlying_type<T>::type;


    template<typename T>
    size_t SequencedHash(const T* vec, size_t count) {
        size_t seed = count;
        for (int i = 0; i < count; i++) {
            seed ^= std::hash()(vec[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }


    template<typename T>
    struct Box
    {
        T XMin = T(0);
        T XMax = T(0);

        T YMin = T(0);
        T YMax = T(0);

        T ZMin = T(0);
        T ZMax = T(0);

        Box() = default;
        Box(T xmin, T xmax,
            T ymin, T ymax,
            T zmin, T zmax)
            : XMin(xmin), XMax(xmax)
            , YMin(ymin), YMax(ymax)
            , ZMin(zmin), ZMax(zmax)
        {}
        Box(T xmin, T xmax,
            T ymin, T ymax)
            : XMin(xmin), XMax(xmax)
            , YMin(ymin), YMax(ymax)
        {}
        Box(T xmin, T xmax)
            : XMin(xmin), XMax(xmax)
        {}

        bool IsEmpty() const {
            static T _eps = std::numeric_limits<T>::epsilon();
            return FLOAT_EPS_EQ(XMin, XMax, _eps) 
                && FLOAT_EPS_EQ(YMin, YMax, _eps) 
                && FLOAT_EPS_EQ(ZMin, ZMax, _eps);
        }
    };
}