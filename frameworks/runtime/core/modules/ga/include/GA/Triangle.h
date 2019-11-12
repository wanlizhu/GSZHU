#pragma once

#include "Common.h"

namespace GA2
{
    template<typename T, int N>
    class Triangle
    {
    public:
        using Scalar = T;
        using VEC = Vector<T, N>;

        Triangle() = default;
        Triangle(const VEC& v0, const VEC& v1, const VEC& v2)
            : mVertices{ v0, v1, v2 }
        {}
        Triangle(const std::array<VEC, 3>& list)
            : mVertices(list)
        {}

        DEFINE_GETTERS(Vertices, std::array<VEC, 3>)
        DEFINE_TRIVIAL_COMPARISONS(Triangle)

        operator bool() const {
            return !(mVertices[0] == mVertices[1]) &&
                   !(mVertices[1] == mVertices[2]) &&
                   !(mVertices[0] == mVertices[2]);
        }

        bool operator==(const TriangleT& tri) const { return mVertices == tri.mVertices; }
        bool operator< (const TriangleT& tri) const { return mVertices <  tri.mVertices; }

    private:
        std::array<VEC, 3> mVertices = { VEC::Zero(), VEC::UnitX(), VEC::UnitY() };
    };
}