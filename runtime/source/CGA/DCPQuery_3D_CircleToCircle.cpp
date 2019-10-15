#include "GE2/CGA/internal/DCPQuery_3D_CircleToCircle.h"
#include "GE2/CGA/internal/Polynomial.h"
#include "GE2/CGA/internal/RootsPolynomial.h"

namespace CGA
{
    class DCPQuery<Circle, Circle>::SCPolynomial
    {
    public:
        SCPolynomial() = default;
        SCPolynomial(double oneTerm, double cosTerm, double sinTerm) 
        {
            mPoly[0] = Polynomial{ oneTerm, cosTerm };
            mPoly[1] = Polynomial{ sinTerm };
        }

        inline const Polynomial& operator[] (unsigned int i) const { return mPoly[i]; }
        inline Polynomial& operator[] (unsigned int i) { return mPoly[i]; }

        SCPolynomial operator+(const SCPolynomial& object) const
        {
            SCPolynomial result;
            result.mPoly[0] = mPoly[0] + object.mPoly[0];
            result.mPoly[1] = mPoly[1] + object.mPoly[1];
            return result;
        }

        SCPolynomial operator-(const SCPolynomial& object) const
        {
            SCPolynomial result;
            result.mPoly[0] = mPoly[0] - object.mPoly[0];
            result.mPoly[1] = mPoly[1] - object.mPoly[1];
            return result;
        }

        SCPolynomial operator*(const SCPolynomial& object) const
        {
            Polynomial omcsqr{ 1.0, 0.0, -1.0 };  // 1 - c^2
            SCPolynomial result;
            result.mPoly[0] = mPoly[0] * object.mPoly[0] + omcsqr * mPoly[1] * object.mPoly[1];
            result.mPoly[1] = mPoly[0] * object.mPoly[1] + mPoly[1] * object.mPoly[0];
            return result;
        }

        SCPolynomial operator*(double scalar) const
        {
            SCPolynomial result;
            result.mPoly[0] = scalar * mPoly[0];
            result.mPoly[1] = scalar * mPoly[1];
            return result;
        }

    private:
        Polynomial mPoly[2]; // poly0(c) + s * poly1(c)
    };

    struct DCPQuery<Circle, Circle>::ClosestInfo
    {
        bool equidistant = false;
        double squaredDistance = 0.0;
        Point circle0Closest;
        Point circle1Closest;

        inline bool operator<(const ClosestInfo& info) const 
        { return squaredDistance < info.squaredDistance; }
    };

    DCPQuery<Circle, Circle>::ComputeResult 
        DCPQuery<Circle, Circle>::operator()(const Circle& circle0, const Circle& circle1)
    {
        ComputeResult result;
        const Vector vzero = Vector::Zero();
        const double zero = 0.0;

        Vector N0 = circle0.GetNormal();
        Vector N1 = circle1.GetNormal();
        double r0 = circle0.GetRadius();
        double r1 = circle1.GetRadius();
        Vector D = circle1.GetCenter() - circle0.GetCenter();
        Vector N0xN1 = N0.cross(N1);

        if (N0xN1 != vzero)
        {
            // Get parameters for constructing the degree-8 polynomial phi.
            const double one = 1.0;
            const double two = 2.0;
            double r0sqr = r0 * r0;
            double r1sqr = r1 * r1;

            // Compute U1 and V1 for the plane of circle1.
            Vector basis[3];
            basis[0] = circle1.GetNormal();
            ComputeOrthogonalComplement(1, basis);
            Vector U1 = basis[1];
            Vector V1 = basis[2];

            // Construct the polynomial phi(cos(theta)).
            Vector N0xD = N0.cross(D);
            Vector N0xU1 = N0.cross(U1);
            Vector N0xV1 = N0.cross(V1);
            double a0 = r1 * D.dot(U1);
            double a1 = r1 * D.dot(V1);
            double a2 = N0xD.dot(N0xD);
            double a3 = r1 * N0xD.dot(N0xU1);
            double a4 = r1 * N0xD.dot(N0xV1);
            double a5 = r1sqr * N0xU1.dot(N0xU1);
            double a6 = r1sqr * N0xU1.dot(N0xV1);
            double a7 = r1sqr * N0xV1.dot(N0xV1);
            Polynomial p0{ a2 + a7, two * a3, a5 - a7 };
            Polynomial p1{ two * a4, two * a6 };
            Polynomial p2{ zero, a1 };
            Polynomial p3{ -a0 };
            Polynomial p4{ -a6, a4, two * a6 };
            Polynomial p5{ -a3, a7 - a5 };
            Polynomial tmp0{ one, zero, -one };
            Polynomial tmp1 = p2 * p2 + tmp0 * p3 * p3;
            Polynomial tmp2 = two * p2 * p3;
            Polynomial tmp3 = p4 * p4 + tmp0 * p5 * p5;
            Polynomial tmp4 = two * p4 * p5;
            Polynomial p6 = p0 * tmp1 + tmp0 * p1 * tmp2 - r0sqr * tmp3;
            Polynomial p7 = p0 * tmp2 + p1 * tmp1 - r0sqr * tmp4;

            unsigned int const maxIterations = MAX_BISECTIONS_GENERIC;
            double roots[8], sn, temp;
            int i, degree, numRoots;

            // The RootsPolynomial<Real>::Find(...) function currently does not
            // combine duplicate roots.  We need only the unique ones here.
            std::set<double> uniqueRoots;

            std::array<std::pair<double, double>, 16> pairs;
            int numPairs = 0;
            if (p7.GetDegree() > 0 || p7[0] != zero)
            {
                // H(cs,sn) = p6(cs) + sn * p7(cs)
                Polynomial phi = p6 * p6 - tmp0 * p7 * p7;
                degree = static_cast<int>(phi.GetDegree());
                assert(degree > 0);
                numRoots = RootsPolynomial::Find(degree, &phi[0],
                                                 maxIterations, roots);
                for (i = 0; i < numRoots; ++i)
                {
                    uniqueRoots.insert(roots[i]);
                }

                for (auto cs : uniqueRoots)
                {
                    if (std::abs(cs) <= one)
                    {
                        temp = p7(cs);
                        if (temp != zero)
                        {
                            sn = -p6(cs) / temp;
                            pairs[numPairs++] = std::make_pair(cs, sn);
                        }
                        else
                        {
                            temp = std::max(one - cs * cs, zero);
                            sn = std::sqrt(temp);
                            pairs[numPairs++] = std::make_pair(cs, sn);
                            if (sn != zero)
                            {
                                pairs[numPairs++] = std::make_pair(cs, -sn);
                            }
                        }
                    }
                }
            }
            else
            {
                // H(cs,sn) = p6(cs)
                degree = static_cast<int>(p6.GetDegree());
                assert(degree > 0);
                numRoots = RootsPolynomial::Find(degree, &p6[0],
                                                 maxIterations, roots);
                for (i = 0; i < numRoots; ++i)
                {
                    uniqueRoots.insert(roots[i]);
                }

                for (auto cs : uniqueRoots)
                {
                    if (std::abs(cs) <= one)
                    {
                        temp = std::max(one - cs * cs, zero);
                        sn = std::sqrt(temp);
                        pairs[numPairs++] = std::make_pair(cs, sn);
                        if (sn != zero)
                        {
                            pairs[numPairs++] = std::make_pair(cs, -sn);
                        }
                    }
                }
            }

            std::array<ClosestInfo, 16> candidates;
            for (i = 0; i < numPairs; ++i)
            {
                ClosestInfo& info = candidates[i];
                Vector delta = D + r1 * (pairs[i].first * U1 + pairs[i].second * V1);
                info.circle1Closest = circle0.GetCenter() + delta;
                double N0dDelta = N0.dot(delta);
                double lenN0xDelta = N0.cross(delta).norm();
                if (lenN0xDelta > 0)
                {
                    double diff = lenN0xDelta - r0;
                    info.squaredDistance = N0dDelta * N0dDelta + diff * diff;
                    delta -= N0dDelta * circle0.GetNormal();
                    delta.normalize();
                    info.circle0Closest = circle0.GetCenter() + r0 * delta;
                    info.equidistant = false;
                }
                else
                {
                    Vector r0U0 = r0 * GetOrthogonal(N0, true);
                    Vector diff = delta - r0U0;
                    info.squaredDistance = diff.dot(diff);
                    info.circle0Closest = circle0.GetCenter() + r0U0;
                    info.equidistant = true;
                }
            }

            std::sort(candidates.begin(), candidates.begin() + numPairs);

            result.numClosestPoints = 1;
            result.squaredDistance = candidates[0].squaredDistance;
            result.circle0Closest[0] = candidates[0].circle0Closest;
            result.circle1Closest[0] = candidates[0].circle1Closest;
            result.equidistant = candidates[0].equidistant;
            if (numRoots > 1 &&
                candidates[1].squaredDistance == candidates[0].squaredDistance)
            {
                result.numClosestPoints = 2;
                result.circle0Closest[1] = candidates[1].circle0Closest;
                result.circle1Closest[1] = candidates[1].circle1Closest;
            }
        }
        else
        {
            // The planes of the circles are parallel.  Whether the planes are the
            // same or different, the problem reduces to determining how two
            // circles in the same plane are separated, tangent with one circle
            // outside the other, overlapping, or one circle contained inside the
            // other circle.
            ComputeParallelPlanes(circle0, circle1, D, &result);
        }

        result.distance = std::sqrt(result.squaredDistance);
        return result;
    }

    void DCPQuery<Circle, Circle>::ComputeParallelPlanes(const Circle& circle0,
                                                         const Circle& circle1,
                                                         const Vector& D,
                                                         DCPQuery<Circle, Circle>::ComputeResult* result)
    {
        double N0dD = circle0.GetNormal().dot(D);
        Vector normProj = N0dD * circle0.GetNormal();
        Vector compProj = D - normProj;
        Vector U = compProj;
        double d = Normalize(U);

        // The configuration is determined by the relative location of the
        // intervals of projection of the circles on to the D-line.  Circle0
        // projects to [-r0,r0] and circle1 projects to [d-r1,d+r1].
        double r0 = circle0.GetRadius();
        double r1 = circle1.GetRadius();
        double dmr1 = d - r1;
        double distance;

        if (dmr1 >= r0)  // d >= r0 + r1
        {
            // The circles are separated (d > r0 + r1) or tangent with one
            // outside the other (d = r0 + r1).
            distance = dmr1 - r0;
            result->numClosestPoints = 1;
            result->circle0Closest[0] = circle0.GetCenter() + r0 * U;
            result->circle1Closest[0] = circle1.GetCenter() - r1 * U;
            result->equidistant = false;
        }
        else // d < r0 + r1
        {
            // The cases implicitly use the knowledge that d >= 0.
            double dpr1 = d + r1;
            if (dpr1 <= r0)
            {
                // Circle1 is inside circle0.
                distance = r0 - dpr1;
                result->numClosestPoints = 1;
                if (d > 0.0)
                {
                    result->circle0Closest[0] = circle0.GetCenter() + r0 * U;
                    result->circle1Closest[0] = circle1.GetCenter() + r1 * U;
                    result->equidistant = false;
                }
                else
                {
                    // The circles are concentric, so U = (0,0,0).  Construct
                    // a vector perpendicular to N0 to use for closest points.
                    U = GetOrthogonal(circle0.GetNormal(), true);
                    result->circle0Closest[0] = circle0.GetCenter() + r0 * U;
                    result->circle1Closest[0] = circle1.GetCenter() + r1 * U;
                    result->equidistant = true;
                }
            }
            else if (dmr1 <= -r0)
            {
                // Circle0 is inside circle1.
                distance = -r0 - dmr1;
                result->numClosestPoints = 1;
                if (d > 0.0)
                {
                    result->circle0Closest[0] = circle0.GetCenter() - r0 * U;
                    result->circle1Closest[0] = circle1.GetCenter() - r1 * U;
                    result->equidistant = false;
                }
                else
                {
                    // The circles are concentric, so U = (0,0,0).  Construct
                    // a vector perpendicular to N0 to use for closest points.
                    U = GetOrthogonal(circle0.GetNormal(), true);
                    result->circle0Closest[0] = circle0.GetCenter() + r0 * U;
                    result->circle1Closest[0] = circle1.GetCenter() + r1 * U;
                    result->equidistant = true;
                }
            }
            else
            {
                // The circles are overlapping.  The two points of intersection
                // are C0 + s*(C1-C0) +/- h*Cross(N,U), where
                // s = (1 + (r0^2 - r1^2)/d^2)/2 and h = sqrt(r0^2 - s^2 * d^2).
                double r0sqr = r0 * r0;
                double r1sqr = r1 * r1;
                double dsqr = d * d;
                double s = (1.0 + (r0sqr - r1sqr) / dsqr) / 2.0;
                double arg = std::max(r0sqr - dsqr * s * s, 0.0);
                double h = std::sqrt(arg);
                Vector midpoint = circle0.GetCenter() + s * compProj;
                Vector hNxU = h * circle0.GetNormal().cross(U);
                distance = 0.0;
                result->numClosestPoints = 2;
                result->circle0Closest[0] = midpoint + hNxU;
                result->circle0Closest[1] = midpoint - hNxU;
                result->circle1Closest[0] = result->circle0Closest[0] + normProj;
                result->circle1Closest[1] = result->circle0Closest[1] + normProj;
                result->equidistant = false;
            }
        }

        result->squaredDistance = distance * distance + N0dD * N0dD;
    }
}