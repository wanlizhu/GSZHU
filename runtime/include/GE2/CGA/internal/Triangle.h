#pragma once

#include "GE2/Utilities.h"

namespace GE2::CGA
{
    template<int N>
    class TriangleT
    {
        using PNTLIST = std::array<VectorT<N>, 3>;
    public:
        using PNT = PointT<N>;
        using VEC = VectorT<N>;

        // The default constructor sets the
        // vertices to (0,..,0), (1,0,...,0), and (0,1,0,...,0).
        TriangleT()
            : mVertices{ PNT::Zero(), PNT::UnitX(), PNT::UnitY() }
        {}
        TriangleT(const PNT& v0, const PNT& v1, const PNT& v2)
            : mVertices{ v0, v1, v2 }
        {}
        TriangleT(const PNTLIST& vs)
            : mVertices(vs)
        {}

        PNTLIST&       GetVertices()       { return mVertices; }
        const PNTLIST& GetVertices() const { return mVertices; }

        PNT&       operator[](int i)       { return mVertices[i]; }
        const PNT& operator[](int i) const { return mVertices[i]; }

        operator bool() const {
            return !(mVertices[0] == mVertices[1]) &&
                   !(mVertices[1] == mVertices[2]) &&
                   !(mVertices[0] == mVertices[2]);
        }

        bool operator==(const TriangleT& tri) const { return mVertices == tri.mVertices; }
        bool operator!=(const TriangleT& tri) const { return mVertices != tri.mVertices; }
        bool operator< (const TriangleT& tri) const { return mVertices <  tri.mVertices; }
        bool operator<=(const TriangleT& tri) const { return mVertices <= tri.mVertices; }
        bool operator> (const TriangleT& tri) const { return mVertices >  tri.mVertices; }
        bool operator>=(const TriangleT& tri) const { return mVertices >= tri.mVertices; }

    private:
        PNTLIST mVertices;
    };

    using Triangle   = TriangleT<3>;
    using Triangle2D = TriangleT<2>;
}