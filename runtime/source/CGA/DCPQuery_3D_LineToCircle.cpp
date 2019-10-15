#include "GE2/CGA/internal/DCPQuery_3D_LineToCircle.h"
#include "GE2/CGA/internal/RootsPolynomial.h"
#include "GE2/CGA/internal/RootsBisection.h"

namespace CGA
{
    struct DCPQuery<Line, Circle>::ClosestInfo
    {
        double squaredDistance = 0.0;
        Vector lineClosest;
        Vector circleClosest;
        bool equidistant = false;

        bool operator< (ClosestInfo const& info) const
        {
            return squaredDistance < info.squaredDistance;
        }
    };

    DCPQuery<Line, Circle>::ComputeResult 
        DCPQuery<Line, Circle>::operator()(const Line& line, const Circle& circle, bool robust)
    {
        DCPQuery<Line, Circle>::ComputeResult result;
        if (robust)
            ComputeDistance3D_NonPoly(line, circle, &result);
        else
            ComputeDistance3D(line, circle, &result);
        return result;
    }

    void DCPQuery<Line, Circle>::ComputeDistance3D(const Line& line, const Circle& circle, ComputeResult* result)
    {
        Vector const vzero = Vector::Zero();
        double const zero = 0.0;

        Vector D = line.GetOrigin() - circle.GetCenter();
        Vector NxM = circle.GetNormal().cross(line.GetDirection());
        Vector NxD = circle.GetNormal().cross(D);
        double t;

        if (NxM != vzero)
        {
            if (NxD != vzero)
            {
                double NdM = circle.GetNormal().dot(line.GetDirection());
                if (NdM != zero)
                {
                    // H(t) = (a*t^2 + 2*b*t + c)*(t + d)^2 - r^2*(a*t + b)^2
                    //      = h0 + h1*t + h2*t^2 + h3*t^3 + h4*t^4
                    double a = NxM.dot(NxM), b = NxM.dot(NxD);
                    double c = NxD.dot(NxD), d = line.GetDirection().dot(D);
                    double rsqr = circle.GetRadius() * circle.GetRadius();
                    double asqr = a * a, bsqr = b * b, dsqr = d * d;
                    double h0 = c * dsqr - bsqr * rsqr;
                    double h1 = 2 * (c * d + b * dsqr - a * b * rsqr);
                    double h2 = c + 4 * b * d + a * dsqr - asqr * rsqr;
                    double h3 = 2 * (b + a * d);
                    double h4 = a;

                    std::map<double, int> rmMap;
                    RootsPolynomial::SolveQuartic(h0, h1, h2, h3, h4, rmMap);
                    std::array<ClosestInfo, 4> candidates;
                    int numRoots = 0;
                    for (auto const& rm : rmMap)
                    {
                        t = rm.first;
                        ClosestInfo info;
                        Vector NxDelta = NxD + t * NxM;
                        if (NxDelta != vzero)
                        {
                            GetPointPair(line, circle, D, t, info.lineClosest, info.circleClosest);
                            info.equidistant = false;
                        }
                        else
                        {
                            Vector U = GetOrthogonal(circle.GetNormal(), true);
                            info.lineClosest = circle.GetCenter();
                            info.circleClosest = circle.GetCenter() + circle.GetRadius() * U;
                            info.equidistant = true;
                        }
                        Vector diff = info.lineClosest - info.circleClosest;
                        info.squaredDistance = diff.dot(diff);
                        candidates[numRoots++] = info;
                    }

                    std::sort(candidates.begin(), candidates.begin() + numRoots);

                    result->numClosestPairs = 1;
                    result->lineClosest[0] = candidates[0].lineClosest;
                    result->circleClosest[0] = candidates[0].circleClosest;
                    if (numRoots > 1 &&
                        candidates[1].squaredDistance == candidates[0].squaredDistance)
                    {
                        result->numClosestPairs = 2;
                        result->lineClosest[1] = candidates[1].lineClosest;
                        result->circleClosest[1] = candidates[1].circleClosest;
                    }
                }
                else
                {
                    // The line is parallel to the plane of the circle.  The
                    // polynomial has the form H(t) = (t+v)^2*[(t+v)^2-(r^2-u^2)].
                    double u = NxM.dot(D), v = line.GetDirection().dot(D);
                    double discr = circle.GetRadius() * circle.GetRadius() - u * u;
                    if (discr > zero)
                    {
                        result->numClosestPairs = 2;
                        double rootDiscr = std::sqrt(discr);
                        t = -v + rootDiscr;
                        GetPointPair(line, circle, D, t, result->lineClosest[0], result->circleClosest[0]);
                        t = -v - rootDiscr;
                        GetPointPair(line, circle, D, t, result->lineClosest[1], result->circleClosest[1]);
                    }
                    else
                    {
                        result->numClosestPairs = 1;
                        t = -v;
                        GetPointPair(line, circle, D, t, result->lineClosest[0], result->circleClosest[0]);
                    }
                }
            }
            else
            {
                // The line is C+t*M, where M is not parallel to N.  The polynomial
                // is H(t) = |Cross(N,M)|^2*t^2*(t^2 - r^2*|Cross(N,M)|^2), where
                // root t = 0 does not correspond to the global minimum.  The other
                // roots produce the global minimum.
                result->numClosestPairs = 2;
                t = circle.GetRadius() * NxM.stableNorm();
                GetPointPair(line, circle, D, t, result->lineClosest[0], result->circleClosest[0]);
                t = -t;
                GetPointPair(line, circle, D, t, result->lineClosest[1], result->circleClosest[1]);
            }
            result->equidistant = false;
        }
        else
        {
            if (NxD != vzero)
            {
                // The line is A+t*N (perpendicular to plane) but with A != C.
                // The polyhomial is H(t) = |Cross(N,D)|^2*(t + Dot(M,D))^2.
                result->numClosestPairs = 1;
                t = -line.GetDirection().dot(D);
                GetPointPair(line, circle, D, t, result->lineClosest[0], result->circleClosest[0]);
                result->equidistant = false;
            }
            else
            {
                // The line is C+t*N, so C is the closest point for the line and
                // all circle points are equidistant from it.
                Vector U = GetOrthogonal(circle.GetNormal(), true);
                result->numClosestPairs = 1;
                result->lineClosest[0] = circle.GetCenter();
                result->circleClosest[0] = circle.GetCenter() + circle.GetRadius() * U;
                result->equidistant = true;
            }
        }

        Vector diff = result->lineClosest[0] - result->circleClosest[0];
        result->squaredDistance = diff.dot(diff);
        result->distance = std::sqrt(result->squaredDistance);
    }

    void DCPQuery<Line, Circle>::ComputeDistance3D_NonPoly(const Line& line, const Circle& circle, ComputeResult* result)
    {
        // The line is P(t) = B+t*M.  The circle is |X-C| = r with Dot(N,X-C)=0.
        Vector vzero = Vector::Zero();
        double const zero = 0.0;

        Vector D = line.GetOrigin() - circle.GetCenter();
        Vector MxN = line.GetDirection().cross(circle.GetNormal());
        Vector DxN = D.cross(circle.GetNormal());

        double m0sqr = MxN.dot(MxN);
        if (m0sqr > zero)
        {
            // Compute the critical points s for F'(s) = 0.
            Vector P0, P1;
            double s, t;
            int numRoots = 0;
            std::array<double, 3> roots;

            // The line direction M and the plane normal N are not parallel.  Move
            // the line origin B = (b0,b1,b2) to B' = B + lambda*line.GetDirection() =
            // (0,b1',b2').
            double m0 = std::sqrt(m0sqr);
            double rm0 = circle.GetRadius() * m0;
            double lambda = -MxN.dot(DxN) / m0sqr;
            Vector oldD = D;
            D += lambda * line.GetDirection();
            DxN += lambda * MxN;
            double m2b2 = line.GetDirection().dot(D);
            double b1sqr = DxN.dot(DxN);
            if (b1sqr > zero)
            {
                // B' = (0,b1',b2') where b1' != 0.  See Sections 1.1.2 and 1.2.2
                // of the PDF documentation.
                double b1 = std::sqrt(b1sqr);
                double rm0sqr = circle.GetRadius() * m0sqr;
                if (rm0sqr > b1)
                {
                    double const twoThirds = (double)2 / (double)3;
                    double sHat = std::sqrt(std::pow(rm0sqr * b1sqr, twoThirds) - b1sqr) / m0;
                    double gHat = rm0sqr * sHat / std::sqrt(m0sqr * sHat * sHat + b1sqr);
                    double cutoff = gHat - sHat;
                    if (m2b2 <= -cutoff)
                    {
                        s = Bisect(m2b2, rm0sqr, m0sqr, b1sqr, -m2b2,
                                   -m2b2 + rm0);
                        roots[numRoots++] = s;
                        if (m2b2 == -cutoff)
                        {
                            roots[numRoots++] = -sHat;
                        }
                    }
                    else if (m2b2 >= cutoff)
                    {
                        s = Bisect(m2b2, rm0sqr, m0sqr, b1sqr, -m2b2 - rm0,
                                   -m2b2);
                        roots[numRoots++] = s;
                        if (m2b2 == cutoff)
                        {
                            roots[numRoots++] = sHat;
                        }
                    }
                    else
                    {
                        if (m2b2 <= zero)
                        {
                            s = Bisect(m2b2, rm0sqr, m0sqr, b1sqr, -m2b2,
                                       -m2b2 + rm0);
                            roots[numRoots++] = s;
                            s = Bisect(m2b2, rm0sqr, m0sqr, b1sqr, -m2b2 - rm0,
                                       -sHat);
                            roots[numRoots++] = s;
                        }
                        else
                        {
                            s = Bisect(m2b2, rm0sqr, m0sqr, b1sqr, -m2b2 - rm0,
                                       -m2b2);
                            roots[numRoots++] = s;
                            s = Bisect(m2b2, rm0sqr, m0sqr, b1sqr, sHat,
                                       -m2b2 + rm0);
                            roots[numRoots++] = s;
                        }
                    }
                }
                else
                {
                    if (m2b2 < zero)
                    {
                        s = Bisect(m2b2, rm0sqr, m0sqr, b1sqr, -m2b2,
                                   -m2b2 + rm0);
                    }
                    else if (m2b2 > zero)
                    {
                        s = Bisect(m2b2, rm0sqr, m0sqr, b1sqr, -m2b2 - rm0,
                                   -m2b2);
                    }
                    else
                    {
                        s = zero;
                    }
                    roots[numRoots++] = s;
                }
            }
            else
            {
                // The new line origin is B' = (0,0,b2').
                if (m2b2 < zero)
                {
                    s = -m2b2 + rm0;
                    roots[numRoots++] = s;
                }
                else if (m2b2 > zero)
                {
                    s = -m2b2 - rm0;
                    roots[numRoots++] = s;
                }
                else
                {
                    s = -m2b2 + rm0;
                    roots[numRoots++] = s;
                    s = -m2b2 - rm0;
                    roots[numRoots++] = s;
                }
            }

            std::array<ClosestInfo, 4> candidates;
            for (int i = 0; i < numRoots; ++i)
            {
                t = roots[i] + lambda;
                ClosestInfo info;
                Vector NxDelta = circle.GetNormal().cross(oldD + t * line.GetDirection());
                if (NxDelta != vzero)
                {
                    GetPointPair(line, circle, oldD, t, info.lineClosest, info.circleClosest);
                    info.equidistant = false;
                }
                else
                {
                    Vector U = GetOrthogonal(circle.GetNormal(), true);
                    info.lineClosest = circle.GetCenter();
                    info.circleClosest = circle.GetCenter() + circle.GetRadius() * U;
                    info.equidistant = true;
                }
                Vector diff = info.lineClosest - info.circleClosest;
                info.squaredDistance = diff.dot(diff);
                candidates[i] = info;
            }

            std::sort(candidates.begin(), candidates.begin() + numRoots);

            result->numClosestPairs = 1;
            result->lineClosest[0] = candidates[0].lineClosest;
            result->circleClosest[0] = candidates[0].circleClosest;
            if (numRoots > 1
                && candidates[1].squaredDistance == candidates[0].squaredDistance)
            {
                result->numClosestPairs = 2;
                result->lineClosest[1] = candidates[1].lineClosest;
                result->circleClosest[1] = candidates[1].circleClosest;
            }

            result->equidistant = false;
        }
        else
        {
            // The line direction and the plane normal are parallel.
            if (DxN != vzero)
            {
                // The line is A+t*N but with A != C.
                result->numClosestPairs = 1;
                GetPointPair(line, circle, D, -line.GetDirection().dot(D),
                        result->lineClosest[0], result->circleClosest[0]);
                result->equidistant = false;
            }
            else
            {
                // The line is C+t*N, so C is the closest point for the line and
                // all circle points are equidistant from it.
                Vector U = GetOrthogonal(circle.GetNormal(), true);
                result->numClosestPairs = 1;
                result->lineClosest[0] = circle.GetCenter();
                result->circleClosest[0] = circle.GetCenter() + circle.GetRadius() * U;
                result->equidistant = true;
            }
        }

        Vector diff = result->lineClosest[0] - result->circleClosest[0];
        result->squaredDistance = diff.dot(diff);
        result->distance = std::sqrt(result->squaredDistance);
    }

    void DCPQuery<Line, Circle>::GetPointPair(const Line& line, const Circle& circle,
                                              const Point& D, double t,
                                              Point& lineClosest, Point& circleClosest)
    {
        Vector delta = D + t * line.GetDirection();
        lineClosest = circle.GetCenter() + delta;
        delta -= circle.GetNormal().dot(delta) * circle.GetNormal();
        Normalize(delta);
        circleClosest = circle.GetCenter() + circle.GetRadius() * delta;
    }

    double DCPQuery<Line, Circle>::Bisect(double m2b2, double rm0sqr, double m0sqr, double b1sqr,
                                          double smin, double smax)
    {
        std::function<double(double)> G = [&, m2b2, rm0sqr, m0sqr, b1sqr](double s) {
            return s + m2b2 - rm0sqr * s / std::sqrt(m0sqr * s * s + b1sqr);
        };

        // The function is known to be increasing, so we can specify -1 and +1
        // as the function values at the bounding interval endpoints.  The use
        // of 'double' is intentional in case Real is a BSNumber or BSRational
        // type.  We want the bisections to terminate in a reasonable amount of
        // time.
        unsigned int const maxIterations = MAX_BISECTIONS_GENERIC;
        double root;
        RootsBisection::Find(G, smin, smax, -1.0, 1.0, maxIterations, root);
        return root;
    }
}