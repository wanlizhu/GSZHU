#pragma once

#include "Geometry/Distance/DCPQuery.h"
#include "Geometry/Shape/Circle3.h"
#include "Geometry/Algebra/OrthogonalComplement.h"

namespace ZHU
{
    /* https://www.geometrictools.com/Documentation/DistanceToCircle3.pdf
     * Let P be the point in 3d space,
     *     C be the center of the circle,
     *     r be the radius of the circle,
     *     N be the normal of the circle's plane,
     *     Q be the projection of P on the circle's plane,
     *     K be the intersection of CQ and the ciecle, which is also the point closest to P
     *     X be the vector from C to P
     *
     * CASE 1: in general
     *         X = P - C
     *     Q - C = X - (N*X)*N;        // N*X is a scalar, N is an unit vector
     *
     *     K = C + r*(Q - C)/|Q - C|
     *       = C + r*(X - (N*X)*N)/|X - (N*X)*N|
     *
     *     |P - K|^2 = |P - Q|^2 + |Q - K|^2
     *               = (N*X)^2   + (|Q - C|             - r)^2
     *               = (N*X)^2   + (sqrt(X^2 - (N*X)^2) - r)^2
     *               = (N*X)^2   + (|N x D| - r)^2
     *
     * CASE 2: the projection of P is exactly the circle center C
     *     |P - K|^2 = |P - C|^2 + |K - C|^2
     *               = |X|^2 + r^2
     *
     * Either a single point on the circle is closest to 'point', 
     * in which case 'equidistant' is false, 
     * or the entire circle is closest to 'point', 
     * in which case 'equidistant' is true.  
     * In the latter case, the query returns the circle point C+r*U, 
     * where C is the circle center, r is the circle radius, 
     * and U is a vector perpendicular to the normal N for the plane of the circle.
     */
    template<typename T>
    class DCPQuery<T, Vector3<T>, Circle3<T>>
    {
    public:
        struct Result
        {
            T distance;
            T distance2; // squared distance
            Vector3<T> closest;
            bool equidistant;
        };

        Result operator()(const Vector3<T>& point,
                          const Circle3<T>& circle);
    };











    /*
                         TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                               CLASS : DCPQuery
                           ARGUMENTS : <typename T, Vector3<T>, Circle3<T>>
                         GENERAL IMP : NO
              PARTIAL SPECIALIZATION : YES
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */
     template<typename T>
     typename DCPQuery<T, Vector3<T>, Circle3<T>>::Result
     DCPQuery<T, Vector3<T>, Circle3<T>>::operator()(const Vector3<T>& point,
                                                     const Circle3<T>& circle)
     {
         Result result;

         // Projection of P-C onto plane is Q-C = P-C - Dot(N,P-C)*N.
         Vector3<T> PmC = point - circle.mCenter;
         Vector3<T> QmC = PmC - circle.mNormal.dot(PmC) * circle.mNormal;
         T QmC_LEN = QmC.norm();

         if (QmC_LEN > (T)0) {
             result.closest = circle.mCenter + (circle.mRadius / QmC_LEN) * QmC;
             result.equidistant = false;
         }
         else {
             // All circle points are equidistant from P.  Return one of them.
             Vector3<T> basis[3];
             basis[0] = circle.mNormal;
             OrthogonalComplement<T, 3>()(1, &basis[0]);
             result.closest = circle.mCenter + circle.mRadius * basis[1];
             result.equidistant = true;
         }

         Vector3<T> diff = point - result.closest;
         result.distance2 = diff.dot(diff);
         result.distance = std::sqrt(result.distance2);

         return result;
     }
}