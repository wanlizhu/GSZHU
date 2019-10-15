#pragma once

#include "BasicTypes.h"

namespace CGA
{
    // Normalize given vector and return its length
    CGA_API double Normalize(Vector& v, bool robust = true);

    // Compute the cross product and normalize it
    CGA_API Vector UnitCross(const Vector& v0, const Vector& v1, bool robust = true);

    // Gram-Schmidt Orthonormalization
    //
    // Gram-Schmidt orthonormalization to generate orthonormal vectors from the
    // linearly independent inputs.  The function returns the smallest length of
    // the unnormalized vectors computed during the process.  If this value is
    // nearly zero, it is possible that the inputs are linearly dependent (within
    // numerical round-off errors).  On input, 1 <= numElements <= N and v[0]
    // through v[numElements-1] must be initialized.  On output, the vectors
    // v[0] through v[numElements-1] form an orthonormal set.
    CGA_API double Orthonormalize(int numInputs, Vector* vs, bool robust = true);

    // Construct a single vector orthogonal to the nonzero input vector.  If the
    // maximum absolute component occurs at index i, then the orthogonal vector
    // U has u[i] = v[i+1], u[i+1] = -v[i], and all other components zero.  The
    // index addition i+1 is computed modulo N.
    CGA_API Vector GetOrthogonal(const Vector& v, bool unitLength);

    // Construct a new coordinate system based on 1 or 2 known vector(s)
    //
    // Compute a right-handed orthonormal basis for the orthogonal complement
    // of the input vectors.  The function returns the smallest length of the
    // unnormalized vectors computed during the process.  If this value is nearly
    // zero, it is possible that the inputs are linearly dependent (within
    // numerical round-off errors).  On input, numInputs must be 1 or 2 and
    // v[0] through v[numInputs-1] must be initialized.  On output, the
    // vectors v[0] through v[2] form an orthonormal set.
    CGA_API double ComputeOrthogonalComplement(int numInputs, Vector* vs, bool robust = true);

    // In 2D space, locate a point using 3 known points
    //
    // Compute the barycentric coordinates of the point P with respect to the
    // triangle <V0,V1,V2>, P = b0*V0 + b1*V1 + b2*V2, where b0 + b1 + b2 = 1.
    // The return value is 'true' iff {V0,V1,V2} is a linearly independent set.
    // Numerically, this is measured by |det[V0 V1 V2]| <= epsilon.  The values
    // bary[] are valid only when the return value is 'true' but set to zero when
    // the return value is 'false'.
    CGA_API bool ComputeBarycentrics(const Vector2D& p,
                                     const Vector2D& v0,
                                     const Vector2D& v1,
                                     const Vector2D& v2,
                                     double bary[3],
                                     double epsilon = 0.0);

    // In 3D space, locate a point using 4 known points
    //
    // Compute the barycentric coordinates of the point P with respect to the
    // tetrahedron <V0,V1,V2,V3>, P = b0*V0 + b1*V1 + b2*V2 + b3*V3, where
    // b0 + b1 + b2 + b3 = 1.  The return value is 'true' iff {V0,V1,V2,V3} is
    // a linearly independent set.  Numerically, this is measured by
    // |det[V0 V1 V2 V3]| <= epsilon.  The values bary[] are valid only when the
    // return value is 'true' but set to zero when the return value is 'false'.
    CGA_API bool ComputeBarycentrics(const Vector& p, 
                                     const Vector& v0,
                                     const Vector& v1,
                                     const Vector& v2,
                                     const Vector& v3,
                                     double bary[4], 
                                     double epsilon = 0.0);
}