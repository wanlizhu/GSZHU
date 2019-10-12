#include "GE2/CGA/details/LinearAlgebra.h"

namespace CGA
{
    inline Vector2D Perp(const Vector2D& v)
    {
        return Vector2D{ v[1], -v[0] };
    }

    double Normalize(Vector& v, bool robust)
    {
        static const int N = 3;
        double length = 0.0;
        if (robust)
        {
            length = v.stableNorm();
            v.stableNormalize();
        }
        else
        {
            length = v.norm();
            v.normalize();
        }
        return length;
    }

    double Orthonormalize(int numInputs, Vector* vs, bool robust)
    {
        static const int N = 3;
        if (vs && 1 <= numInputs && numInputs <= N)
        {
            double minLength = Normalize(vs[0], robust);
            for (int i = 1; i < numInputs; ++i)
            {
                for (int j = 0; j < i; ++j)
                {
                    double dot = vs[i].dot(vs[j]);
                    vs[i] -= vs[j] * dot;
                }

                double length = Normalize(vs[i], robust);
                if (length < minLength)
                {
                    minLength = length;
                }
            }
            return minLength;
        }

        return 0.0;
    }

    Vector GetOrthogonal(const Vector& v, bool unitLength)
    {
        static const int N = 3;
        double cmax = std::abs(v[0]);
        int imax = 0;
        for (int i = 1; i < N; ++i)
        {
            double c = std::abs(v[i]);
            if (c > cmax)
            {
                cmax = c;
                imax = i;
            }
        }

        Vector result(0.0, 0.0, 0.0);
        int inext = imax + 1;
        if (inext == N)
        {
            inext = 0;
        }
        result[imax] = v[inext];
        result[inext] = -v[imax];
        if (unitLength)
        {
            double sqrDistance = result[imax] * result[imax] + result[inext] * result[inext];
            double invLength = (1.0) / std::sqrt(sqrDistance);
            result[imax] *= invLength;
            result[inext] *= invLength;
        }
        return result;
    }

    double ComputeOrthogonalComplement(int numInputs, Vector* vs, bool robust)
    {
        if (numInputs == 1)
        {
            if (std::abs(vs[0][0]) > std::abs(vs[0][1]))
            {
                vs[1] = { -vs[0][2], 0.0, vs[0][0] };
            }
            else
            {
                vs[1] = { 0.0, vs[0][2], -vs[0][1] };
            }
            numInputs = 2;
        }

        if (numInputs == 2)
        {
            vs[2] = vs[0].cross(vs[1]);
            return Orthonormalize(3, vs, robust);
        }

        return 0.0;
    }

    bool ComputeBarycentrics(const Vector2D& p,
                             const Vector2D& v0,
                             const Vector2D& v1,
                             const Vector2D& v2,
                             double bary[3],
                             double epsilon)
    {
        // Compute the vectors relative to V2 of the triangle.
        Vector2D diff[3] = { v0 - v2, v1 - v2, p - v2 };

        double det = diff[0].dot(Perp(diff[1]));
        if (det < -epsilon || det > epsilon)
        {
            double invDet = (1.0) / det;
            bary[0] = diff[2].dot(Perp(diff[1])) * invDet;
            bary[1] = diff[0].dot(Perp(diff[2])) * invDet;
            bary[2] = 1.0 - bary[0] - bary[1];
            return true;
        }

        for (int i = 0; i < 3; ++i)
        {
            bary[i] = 0.0;
        }
        return false;
    }

    bool ComputeBarycentrics(const Vector& p,
                             const Vector& v0,
                             const Vector& v1,
                             const Vector& v2,
                             const Vector& v3,
                             double bary[4],
                             double epsilon)
    {
        // Compute the vectors relative to V3 of the tetrahedron.
        Vector diff[4] = { v0 - v3, v1 - v3, v2 - v3, p - v3 };

        double det = diff[0].dot(diff[1].cross(diff[2]));
        if (det < -epsilon || det > epsilon)
        {
            double invDet = (1.0) / det;
            bary[0] = diff[3].dot(diff[1].cross(diff[2])) * invDet;
            bary[1] = diff[3].dot(diff[2].cross(diff[0])) * invDet;
            bary[2] = diff[3].dot(diff[0].cross(diff[1])) * invDet;
            bary[3] = 1.0 - bary[0] - bary[1] - bary[2];
            return true;
        }

        for (int i = 0; i < 4; ++i)
        {
            bary[i] = 0.0;
        }
        return false;
    }
}