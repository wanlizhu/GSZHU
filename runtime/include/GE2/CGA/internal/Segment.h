#pragma once

#include "GE2/Utilities.h"

namespace GE2::CGA
{
    template<int N>
    class SegmentT
    {
        using PNTLIST = std::array<PointT<N>, 2>;
    public:
        using PNT = PointT<N>;
        using VEC = VectorT<N>;

        // The default constructor sets p0 to (-1,0,...,0) and p1 to (1,0,...,0).  
        // NOTE:  If you set p0 and p1; compute C, D, and e; and then recompute q0 = C-e*D and q1 = C+e*D,
        // numerical round-off errors can lead to q0 not exactly equal to p0
        // and q1 not exactly equal to p1.
        SegmentT()
            : mPoints{ PNT::UnitX(), -PNT::UnitX() }
        {}
        SegmentT(const PNT& p0, const PNT& p1)
            : mPoints{ p0, p1 }
        {}
        SegmentT(const PNTLIST& ps)
            : mPoints(ps)
        {}
        SegmentT(const PNT& center, const VEC& direction, double extent)
        {
            SetCenteredForm(center, direction, extent);
        }

        void SetCenteredForm(const PNT& center, const VEC& direction, double extent)
        {
            mPoints[0] = center - extent * direction;
            mPoints[1] = center + extent * direction;
        }
        void GetCenteredForm(PNT* center, VEC* direction, double* extent) const
        {
            *center = (0.5) * (mPoints[0] + mPoints[1]);
            *direction = mPoints[1] - mPoints[0];
            *extent = (0.5) * Normalize(*direction);
        }

        PNTLIST&       GetPoints()       { return mPoints; }
        const PNTLIST& GetPoints() const { return mPoints; }

        PNT&       operator[](int i)       { return mPoints[i]; }
        const PNT& operator[](int i) const { return mPoints[i]; }

        operator bool() const { return mPoints[0] != mPoints[1]; }

        bool operator==(const SegmentT& seg) const { return mPoints == seg.mPoints; }
        bool operator!=(const SegmentT& seg) const { return mPoints != seg.mPoints; }
        bool operator< (const SegmentT& seg) const { return mPoints <  seg.mPoints; }
        bool operator<=(const SegmentT& seg) const { return mPoints <= seg.mPoints; }
        bool operator> (const SegmentT& seg) const { return mPoints >  seg.mPoints; }
        bool operator>=(const SegmentT& seg) const { return mPoints >= seg.mPoints; }

    private:
        PNTLIST mPoints;
    };

    using Segment   = SegmentT<3>;
    using Segment2D = SegmentT<2>;
}