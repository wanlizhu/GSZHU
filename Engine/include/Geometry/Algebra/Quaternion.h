#pragma once

#include "Geometry/Algebra/Matrix.h"
#include "Geometry/Algebra/ChebyshevRatio.h"

namespace ZHU
{
    /* A quaternion is of the form
     *   q = x * i + y * j + z * k + w * 1 = x * i + y * j + z * k + w
     * where w, x, y, and z are real numbers.  The scalar and vector parts are
     *   Vector(q) = x * i + y * j + z * k
     *   Scalar(q) = w
     *   q = Vector(q) + Scalar(q)
     * I assume that you are familiar with the arithmetic and algebraic properties
     * of quaternions.  See
     * https://www.geometrictools.com/Documentation/Quaternions.pdf
     * 
     * The quaternions are of the form q = x*i + y*j + z*k + w.  In tuple form, q = (x,y,z,w).
     */
    template<typename T>
    class Quaternion
    {
    public:
        Quaternion() = default;
        Quaternion(T x, T y, T z, T w) : mArray({ x, y, z, w }) {}

        inline const T& operator[](int i) const           { return mArray[i]; }
        inline       T& operator[](int i)                 { return mArray[i]; }
        inline bool operator==(const Quaternion& q) const { return mArray==q.mArray; }
        inline bool operator!=(const Quaternion& q) const { return mArray!=q.mArray; }
        inline bool operator< (const Quaternion& q) const { return mArray< q.mArray; }
        inline bool operator<=(const Quaternion& q) const { return mArray<= q.mArray; }
        inline bool operator> (const Quaternion& q) const { return mArray> q.mArray; }
        inline bool operator>=(const Quaternion& q) const { return mArray>=q.mArray; }

        inline Quaternion  operator-() const;
        inline Quaternion  operator+(const Quaternion& q) const;
        inline Quaternion  operator-(const Quaternion& q) const;
        inline Quaternion  operator*(const Quaternion& q) const; // is not commutative
        inline Quaternion  operator*(T scalar) const;
        friend inline Quaternion operator*(T scalar, const Quaternion& q) { return q * scalar; }
        inline Quaternion  operator/(T scalar) const;
        inline Quaternion& operator+=(const Quaternion& q);
        inline Quaternion& operator-=(const Quaternion& q);
        inline Quaternion& operator*=(const Quaternion& q);
        inline Quaternion& operator*=(T scalar);
        inline Quaternion& operator/=(T scalar);

        static Quaternion Zero()     { return Quaternion((T)0, (T)0, (T)0, (T)0); }
        static Quaternion I()        { return Quaternion((T)1, (T)0, (T)0, (T)0); }
        static Quaternion J()        { return Quaternion((T)0, (T)1, (T)0, (T)0); }
        static Quaternion K()        { return Quaternion((T)0, (T)0, (T)1, (T)0); }
        static Quaternion Identity() { return Quaternion((T)0, (T)0, (T)0, (T)1); }

    protected:
        std::array<T, 4> mArray;
    };


    template<typename T>
    T Dot(const Quaternion<T>& q0, const Quaternion<T>& q1);


    template<typename T>
    T Length(const Quaternion<T>& q);


    template<typename T>
    T Normalize(const Quaternion<T>& q);


    template<typename T>
    Quaternion<T> Inverse(const Quaternion<T>& q);


    template<typename T>
    Quaternion<T> Conjugate(const Quaternion<T>& q);


    template<typename T>
    Vector4<T> Rotate(const Quaternion<T>& q, const Vector4<T>& v);


    /*
     * The spherical linear interpolation (slerp) of unit-length quaternions
     * q0 and q1 for t in [0,1] is
     *     slerp(t,q0,q1) = [sin(t*theta)*q0 + sin((1-t)*theta)*q1]/sin(theta)
     * where theta is the angle between q0 and q1 [cos(theta) = Dot(q0,q1)].
     * This function is a parameterization of the great spherical arc between
     * q0 and q1 on the unit hypersphere.  Moreover, the parameterization is
     * one of normalized arclength--a particle traveling along the arc through
     * time t does so with constant speed.
     *
     * When using slerp in animations involving sequences of quaternions, it is
     * typical that the quaternions are preprocessed so that consecutive ones
     * form an acute angle A in [0,pi/2].  Other preprocessing can help with
     * performance.  See the function comments below.
     *
     * See GteSlerpEstimate.{h,inl} for various approximations, including
     * SLERP<Real>::EstimateRPH that gives good performance and accurate results
     * for preprocessed quaternions.
     *
     * The angle between q0 and q1 is in [0,pi).  There are no angle restrictions
     * restrictions and nothing is precomputed.
     */
    template<typename T>
    Quaternion<T> Slerp(T t, 
                        const Quaternion<T>& q0,
                        const Quaternion<T>& q1);


    /*
     * The angle between q0 and q1 must be in [0,pi/2].  The suffix R is for
     * 'Restricted'.  The preprocessing code is
     *   Quaternion<Real> q[n];  // assuming initialized
     *   for (i0 = 0, i1 = 1; i1 < n; i0 = i1++)
     *   {
     *       cosA = Dot(q[i0], q[i1]);
     *       if (cosA < 0)
     *       {
     *           q[i1] = -q[i1];  // now Dot(q[i0], q[i]1) >= 0
     *       }
     *   }
     */
    template<typename T>
    Quaternion<T> SlerpR(T t,
                         const Quaternion<T>& q0,
                         const Quaternion<T>& q1);


    /*
     * The angle between q0 and q1 must be in [0,pi/2].  The suffix R is for
     * 'Restricted' and the suffix P is for 'Preprocessed'.  The preprocessing
     * code is
     *   Quaternion<Real> q[n];  // assuming initialized
     *   Real cosA[n-1], omcosA[n-1];  // to be precomputed
     *   for (i0 = 0, i1 = 1; i1 < n; i0 = i1++)
     *   {
     *       cs = Dot(q[i0], q[i1]);
     *       if (cosA[i0] < 0)
     *       {
     *           q[i1] = -q[i1];
     *           cs = -cs;
     *       }
     *
     *       // for Quaterion<Real>::SlerpRP
     *       cosA[i0] = cs;
     *
     *       // for SLERP<Real>::EstimateRP
     *       omcosA[i0] = 1 - cs;
     *   }
     */
    template<typename T>
    Quaternion<T> SlerpRP(T t,
                          const Quaternion<T>& q0,
                          const Quaternion<T>& q1);


    /*
     * The angle between q0 and q1 is A and must be in [0,pi/2].  The suffic R
     * is for 'Restricted', the suffix P is for 'Preprocessed' and the suffix
     * H is for 'Half' (the quaternion qh halfway between q0 and q1 is
     * precomputed).  Quaternion qh is slerp(1/2,q0,q1) = (q0+q1)/|q0+q1|, so
     * the angle between q0 and qh is A/2 and the angle between qh and q1 is
     * A/2.  The preprocessing code is
     *   Quaternion<Real> q[n];  // assuming initialized
     *   Quaternion<Real> qh[n-1];  // to be precomputed
     *   Real omcosAH[n-1];  // to be precomputed
     *   for (i0 = 0, i1 = 1; i1 < n; i0 = i1++)
     *   {
     *       cosA = Dot(q[i0], q[i1]);
     *       if (cosA < 0)
     *       {
     *           q[i1] = -q[i1];
     *           cosA = -cosA;
     *       }
     *
     *       // for Quaternion<Real>::SlerpRPH and SLERP<Real>::EstimateRPH
     *       cosAH[i0] = sqrt((1+cosA)/2);
     *       qh[i0] = (q0 + q1) / (2 * cosAH[i0]);
     *
     *       // for SLERP<Real>::EstimateRPH
     *       omcosAH[i0] = 1 - cosAH[i0];
     *   }
     */
    template<typename T>
    Quaternion<T> SlerpRPH(T t,
                           const Quaternion<T>& q0,
                           const Quaternion<T>& q1);















    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                               CLASS : Quaternion
                           ARGUMENTS : <typename T>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template<typename T>
    inline Quaternion<T>  Quaternion<T>::operator-() const
    {
        Quaternion<T> res;
        for (int i = 0; i < 4; i++) {
            res[i] = -mArray[i];
        }
        return res;
    }


    template<typename T>
    inline Quaternion<T>  Quaternion<T>::operator+(const Quaternion<T>& q) const
    {
        return Quaternion<T>(*this) += q;
    }


    template<typename T>
    inline Quaternion<T>  Quaternion<T>::operator-(const Quaternion<T>& q) const
    {
        return Quaternion<T>(*this) -= q;
    }


    template<typename T>
    inline Quaternion<T>  Quaternion<T>::operator*(const Quaternion<T>& q) const
    {
        return Quaternion<T>(*this) *= q;
    }


    template<typename T>
    inline Quaternion<T>  Quaternion<T>::operator*(T scalar) const
    {
        return Quaternion<T>(*this) *= scalar;
    }


    template<typename T>
    inline Quaternion<T>  Quaternion<T>::operator/(T scalar) const
    {
        return Quaternion<T>(*this) /= scalar;
    }


    template<typename T>
    inline Quaternion<T>& Quaternion<T>::operator+=(const Quaternion& q)
    {
        for (int i = 0; i < 4; i++) {
            mArray[i] += q.mArray[i];
        }
        return *this;
    }


    template<typename T>
    inline Quaternion<T>& Quaternion<T>::operator-=(const Quaternion& q)
    {
        for (int i = 0; i < 4; i++) {
            mArray[i] -= q.mArray[i];
        }
        return *this;
    }


    template<typename T>
    inline Quaternion<T>& Quaternion<T>::operator*=(const Quaternion& q)
    {
        /* Multiplication of quaternions.  This operation is not generally
         * commutative; that is, q0*q1 and q1*q0 are not usually the same value.
         * (x0*i + y0*j + z0*k + w0)*(x1*i + y1*j + z1*k + w1)
         * =
         * i*(+x0*w1 + y0*z1 - z0*y1 + w0*x1) +
         * j*(-x0*z1 + y0*w1 + z0*x1 + w0*y1) +
         * k*(+x0*y1 - y0*x1 + z0*w1 + w0*z1) +
         * 1*(-x0*x1 - y0*y1 - z0*z1 + w0*w1)
         */
        const Quaternion<T>& q0 = *this;
        const Quaternion<T>& q1 = q;

        *this = Quaternion<T>
            (
                +q0[0] * q1[3] + q0[1] * q1[2] - q0[2] * q1[1] + q0[3] * q1[0],
                -q0[0] * q1[2] + q0[1] * q1[3] + q0[2] * q1[0] + q0[3] * q1[1],
                +q0[0] * q1[1] - q0[1] * q1[0] + q0[2] * q1[3] + q0[3] * q1[2],
                -q0[0] * q1[0] - q0[1] * q1[1] - q0[2] * q1[2] + q0[3] * q1[3]
            );
        return *this;
    }


    template<typename T>
    inline Quaternion<T>& Quaternion<T>::operator*=(T scalar)
    {
        for (int i = 0; i < 4; i++) {
            mArray[i] *= scalar;
        }
        return *this;
    }


    template<typename T>
    inline Quaternion<T>& Quaternion<T>::operator/=(T scalar)
    {
        if (scalar != (T)0) {
            for (int i = 0; i < 4; ++i) {
                mArray[i] /= scalar;
            }
        }
        else {
            for (int i = 0; i < 4; ++i) {
                mArray[i] = (T)0;
            }
        }
        return *this;
    }










    template<typename T>
    T Dot(const Quaternion<T>& q0, const Quaternion<T>& q1)
    {
        T dot = q0[0] * q1[0];
        for (int i = 1; i < 4; ++i) {
            dot += q0[i] * q1[i];
        }
        return dot;
    }


    template<typename T>
    T Length(const Quaternion<T>& q)
    {
        return std::sqrt(Dot(q, q));
    }


    template<typename T>
    T Normalize(const Quaternion<T>& q)
    {
        T length = std::sqrt(Dot(q, q));
        if (length > (T)0) {
            q /= length;
        }
        else {
            for (int i = 0; i < 4; ++i) {
                q[i] = (T)0;
            }
        }
        return length;
    }


    template<typename T>
    Quaternion<T> Inverse(const Quaternion<T>& q)
    {
        T sqrLen = Dot(q, q);
        if (sqrLen > (T)0) {
            Quaternion<T> inverse = Conjugate(q) / sqrLen;
            return inverse;
        }
        else {
            return Quaternion<T>::Zero();
        }
    }


    template<typename T>
    Quaternion<T> Conjugate(const Quaternion<T>& q)
    {
        return Quaternion<T>(-q[0], -q[1], -q[2], +q[3]);
    }


    template<typename T>
    Vector4<T> Rotate(const Quaternion<T>& q, const Vector4<T>& v)
    {
        Quaternion<T> input(v[0], v[1], v[2], (T)0);
        Quaternion<T> output = q * input * Conjugate(q);
        Vector<4, T> u{ output[0], output[1], output[2], (T)0 };
        return u;
    }


    template<typename T>
    Quaternion<T> Slerp(T t,
                        const Quaternion<T>& q0,
                        const Quaternion<T>& q1)
    {
        T cosA = Dot(q0, q1);
        T sign;
        if (cosA >= (T)0) {
            sign = (T)1;
        }
        else {
            cosA = -cosA;
            sign = (T)-1;
        }

        T f0, f1;
        ChebyshevRatio<T>::Get(t, cosA, f0, f1);
        return q0 * f0 + q1 * (sign * f1);
    }


    template<typename T>
    Quaternion<T> SlerpR(T t,
                         const Quaternion<T>& q0,
                         const Quaternion<T>& q1)
    {
        T f0, f1;
        ChebyshevRatio<T>::Get(t, Dot(q0, q1), f0, f1);
        return q0 * f0 + q1 * f1;
    }


    template<typename T>
    Quaternion<T> SlerpRP(T t,
                          const Quaternion<T>& q0,
                          const Quaternion<T>& q1)
    {
        T f0, f1;
        ChebyshevRatio<T>::Get(t, cosA, f0, f1);
        return q0 * f0 + q1 * f1;
    }


    template<typename T>
    Quaternion<T> SlerpRPH(T t,
                           const Quaternion<T>& q0,
                           const Quaternion<T>& q1)
    {
        T f0, f1;
        T twoT = static_cast<T>(2) * t;
        if (twoT <= static_cast<T>(1)) {
            ChebyshevRatio<T>::Get(twoT, cosAH, f0, f1);
            return q0 * f0 + qh * f1;
        }
        else {
            ChebyshevRatio<T>::Get(twoT - static_cast<T>(1), cosAH, f0, f1);
            return qh * f0 + q1 * f1;
        }
    }
}