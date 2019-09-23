#pragma once

#include "Matrix.h"
#include "Vector.h"
#include "details/ChebyshevRatio.h"
#include <array>

// A quaternion is of the form
//   q = x * i + y * j + z * k + w * 1 = x * i + y * j + z * k + w
// where w, x, y, and z are real numbers.  The scalar and vector parts are
//   Vector(q) = x * i + y * j + z * k
//   Scalar(q) = w
//   q = Vector(q) + Scalar(q)
// I assume that you are familiar with the arithmetic and algebraic properties
// of quaternions. 

namespace GML
{
	template<typename T>
	class Quaternion
	{
	public:
		// The quaternions are of the form q = x*i + y*j + z*k + w.  
		// In tuple form, q = (x,y,z,w).
		Quaternion() = default;
		Quaternion(T x, T y, T z, T w) : mArray{ x, y, z, w } {}

		inline const T& operator[](int i) const { return mArray[i]; }
		inline       T& operator[](int i)       { return mArray[i]; }
		inline const T& x()               const { return mArray[0]; };
		inline 	     T& x()                     { return mArray[0]; };
		inline const T& y()               const { return mArray[1]; };
		inline 	     T& y()                     { return mArray[1]; };
		inline const T& z()               const { return mArray[2]; };
		inline       T& z()                     { return mArray[2]; };
		inline const T& w()               const { return mArray[3]; };
		inline       T& w()                     { return mArray[3]; };

		inline bool operator==(const Quaternion& other) const { return OP_EQ<T, 4>(mArray.data(), other.mArray.data()); }
		inline bool operator!=(const Quaternion& other) const { return !operator==(other); }

		inline Quaternion& make_zero()     { mArray = { (T)0, (T)0, (T)0, (T)0 }; return *this; }
		inline Quaternion& make_identity() { mArray = { (T)0, (T)0, (T)0, (T)1 }; return *this; }

		inline static Quaternion zero()     { return Quaternion().make_zero(); }
		inline static Quaternion identity() { return Quaternion().make_identity() }

	private:
		std::array<T, 4> mArray;
	};




	template<typename T>
	T dot(const Quaternion<T>& q0, const Quaternion<T>& q1)
	{
		T num = q0[0] * q1[0];
		for (int i = 1; i < 4; ++i)
			num += q0[i] * q1[i];
		return num;
	}

	template<typename T>
	T length(const Quaternion<T>& q)
	{
		return std::sqrt(dot(q, q));
	}

	template<typename T>
	Quaternion<T> length(const Quaternion<T>& q)
	{
		Quaternion<T> res(q);
		T len = std::sqrt(dot(q, q));
		if (len > (T)0)
			res /= len;
		else for (int i = 0; i < 4; i++)
			res[i] = (T)0;
		return res;
	}

	// Multiplication of quaternions.  This operation is not generally
	// commutative; that is, q0*q1 and q1*q0 are not usually the same value.
	// (x0*i + y0*j + z0*k + w0)*(x1*i + y1*j + z1*k + w1)
	// =
	// i*(+x0*w1 + y0*z1 - z0*y1 + w0*x1) +
	// j*(-x0*z1 + y0*w1 + z0*x1 + w0*y1) +
	// k*(+x0*y1 - y0*x1 + z0*w1 + w0*z1) +
	// 1*(-x0*x1 - y0*y1 - z0*z1 + w0*w1)
	template<typename T>
	Quaternion<T> operator*(const Quaternion<T>& q0, const Quaternion<T>& q1)
	{
		// (x0*i + y0*j + z0*k + w0)*(x1*i + y1*j + z1*k + w1)
		// =
		// i*(+x0*w1 + y0*z1 - z0*y1 + w0*x1) +
		// j*(-x0*z1 + y0*w1 + z0*x1 + w0*y1) +
		// k*(+x0*y1 - y0*x1 + z0*w1 + w0*z1) +
		// 1*(-x0*x1 - y0*y1 - z0*z1 + w0*w1)

		return Quaternion<T>(
				+q0[0] * q1[3] + q0[1] * q1[2] - q0[2] * q1[1] + q0[3] * q1[0],
				-q0[0] * q1[2] + q0[1] * q1[3] + q0[2] * q1[0] + q0[3] * q1[1],
				+q0[0] * q1[1] - q0[1] * q1[0] + q0[2] * q1[3] + q0[3] * q1[2],
				-q0[0] * q1[0] - q0[1] * q1[1] - q0[2] * q1[2] + q0[3] * q1[3]);
	}

	// For a nonzero quaternion q = (x,y,z,w), inv(q) = (-x,-y,-z,w)/|q|^2, where
	// |q| is the length of the quaternion.  When q is zero, the function returns
	// zero, which is considered to be an improbable case.
	template<typename T>
	Quaternion<T> inverse(const Quaternion<T>& q)
	{
		T sqrLen = dot(q, q);
		if (sqrLen > (T)0)
		{
			Quaternion<T> inv = conjugate(q) / sqrLen;
			return inv;
		}
		else
		{
			return Quaternion<T>::zero();
		}
	}

	// The conjugate of q = (x,y,z,w) is conj(q) = (-x,-y,-z,w).
	template<typename T>
	Quaternion<T> conjugate(const Quaternion<T>& q)
	{
		return Quaternion<T>(-q[0], -q[1], -q[2], q[3]);
	}

	// Rotate a vector using quaternion multiplication.  The input quaternion must
	// be unit length.
	template<typename T>
	Vector4<T> rotate(const Quaternion<T>& q, const Vector4<T>& v)
	{
		Quaternion<T> input(v[0], v[1], v[2], (T)0);
		Quaternion<T> output = q * input * conjugate(q);
		Vector<4, T> u{ output[0], output[1], output[2], (T)0 };
		return u;
	}

	// The spherical linear interpolation (slerp) of unit-length quaternions
	// q0 and q1 for t in [0,1] is
	//     slerp(t,q0,q1) = [sin(t*theta)*q0 + sin((1-t)*theta)*q1]/sin(theta)
	// where theta is the angle between q0 and q1 [cos(theta) = Dot(q0,q1)].
	// This function is a parameterization of the great spherical arc between
	// q0 and q1 on the unit hypersphere.  Moreover, the parameterization is
	// one of normalized arclength--a particle traveling along the arc through
	// time t does so with constant speed.
	//
	// When using slerp in animations involving sequences of quaternions, it is
	// typical that the quaternions are preprocessed so that consecutive ones
	// form an acute angle A in [0,pi/2].  Other preprocessing can help with
	// performance.  See the function comments below.
	//
	// See GteSlerpEstimate.{h,inl} for various approximations, including
	// SLERP<Real>::EstimateRPH that gives good performance and accurate results
	// for preprocessed quaternions.
	//
	// The angle between q0 and q1 is in [0,pi).  There are no angle restrictions
	// restrictions and nothing is precomputed.
	template<typename T>
	Quaternion<T> slerp(T t, const Quaternion<T>& q0, const Quaternion<T>& q1)
	{
		T cosA = dot(q0, q1);
		T sign;
		if (cosA >= (T)0)
		{
			sign = (T)1;
		}
		else
		{
			cosA = -cosA;
			sign = (T)-1;
		}

		T f0;
		T f1;
		ChebyshevRatio<T>::Get(t, cosA, &f0, &f1);
		return q0 * f0 + q1 * (sign * f1);
	}

	// The angle between q0 and q1 must be in [0,pi/2].  The suffix R is for
	// 'Restricted'.  The preprocessing code is
	//   Quaternion<Real> q[n];  // assuming initialized
	//   for (i0 = 0, i1 = 1; i1 < n; i0 = i1++)
	//   {
	//       cosA = Dot(q[i0], q[i1]);
	//       if (cosA < 0)
	//       {
	//           q[i1] = -q[i1];  // now Dot(q[i0], q[i]1) >= 0
	//       }
	//   }
	template<typename T>
	Quaternion<T> slerp_r(T t, const Quaternion<T>& q0, const Quaternion<T>& q1)
	{
		T f0;
		T f1;
		ChebyshevRatio<T>::Get(t, dot(q0, q1), &f0, &f1);
		return q0 * f0 + q1 * f1;
	}

	// The angle between q0 and q1 must be in [0,pi/2].  The suffix R is for
	// 'Restricted' and the suffix P is for 'Preprocessed'.  The preprocessing
	// code is
	//   Quaternion<Real> q[n];  // assuming initialized
	//   Real cosA[n-1], omcosA[n-1];  // to be precomputed
	//   for (i0 = 0, i1 = 1; i1 < n; i0 = i1++)
	//   {
	//       cs = Dot(q[i0], q[i1]);
	//       if (cosA[i0] < 0)
	//       {
	//           q[i1] = -q[i1];
	//           cs = -cs;
	//       }
	//
	//       // for Quaterion<Real>::SlerpRP
	//       cosA[i0] = cs;
	//
	//       // for SLERP<Real>::EstimateRP
	//       omcosA[i0] = 1 - cs;
	//   }
	template<typename T>
	Quaternion<T> slerp_rp(T t, const Quaternion<T>& q0, const Quaternion<T>& q1, T cosA)
	{
		T f0;
		T f1;
		ChebyshevRatio<T>::Get(t, cosA, &f0, &f1);
		return q0 * f0 + q1 * f1;
	}

	// The angle between q0 and q1 is A and must be in [0,pi/2].  The suffic R
	// is for 'Restricted', the suffix P is for 'Preprocessed' and the suffix
	// H is for 'Half' (the quaternion qh halfway between q0 and q1 is
	// precomputed).  Quaternion qh is slerp(1/2,q0,q1) = (q0+q1)/|q0+q1|, so
	// the angle between q0 and qh is A/2 and the angle between qh and q1 is
	// A/2.  The preprocessing code is
	//   Quaternion<Real> q[n];  // assuming initialized
	//   Quaternion<Real> qh[n-1];  // to be precomputed
	//   Real omcosAH[n-1];  // to be precomputed
	//   for (i0 = 0, i1 = 1; i1 < n; i0 = i1++)
	//   {
	//       cosA = Dot(q[i0], q[i1]);
	//       if (cosA < 0)
	//       {
	//           q[i1] = -q[i1];
	//           cosA = -cosA;
	//       }
	//
	//       // for Quaternion<Real>::SlerpRPH and SLERP<Real>::EstimateRPH
	//       cosAH[i0] = sqrt((1+cosA)/2);
	//       qh[i0] = (q0 + q1) / (2 * cosAH[i0]);
	//
	//       // for SLERP<Real>::EstimateRPH
	//       omcosAH[i0] = 1 - cosAH[i0];
	//   }
	template<typename T>
	Quaternion<T> slerp_rph(T t, const Quaternion<T>& q0, const Quaternion<T>& q1,
						   const Quaternion<T>& qh, T cosAH)
	{
		T f0;
		T f1;
		T twoT = static_cast<T>(2) * t;
		if (twoT <= static_cast<T>(1))
		{
			ChebyshevRatio<T>::Get(twoT, cosAH, &f0, &f1);
			return q0 * f0 + qh * f1;
		}
		else
		{
			ChebyshevRatio<T>::Get(twoT - static_cast<T>(1), cosAH, &f0, &f1);
			return qh * f0 + q1 * f1;
		}
	}




	template<typename T>
	Quaternion<T> operator-(const Quaternion<T>& q)
	{
		Quaternion<T> result;
		for (int i = 0; i < 4; ++i)
			result[i] = -q[i];
		return result;
	}

	// Linear algebraic operations.
	template<typename T>
	Quaternion<T> operator+(Quaternion<T> const& q0, Quaternion<T> const& q1)
	{
		Quaternion<T> result = q0;
		return result += q1;
	}

	template<typename T>
	Quaternion<T> operator-(const Quaternion<T>& q0, const Quaternion<T>& q1)
	{
		Quaternion<T> result = q0;
		return result -= q1;
	}

	template<typename T>
	Quaternion<T> operator*(const Quaternion<T>& q, T scalar)
	{
		Quaternion<Real> result = q;
		return result *= scalar;
	}

	template<typename T>
	Quaternion<T> operator*(T scalar, const Quaternion<T>& q)
	{
		Quaternion<Real> result = q;
		return result *= scalar;
	}

	template<typename T>
	Quaternion<T> operator/(const Quaternion<T>& q, T scalar)
	{
		Quaternion<Real> result = q;
		return result /= scalar;
	}

	template<typename T>
	Quaternion<T>& operator+=(Quaternion<T>& q0, const Quaternion<T>& q1)
	{
		for (int i = 0; i < 4; i++)
			q0[i] += q1[i];
		return q0;
	}

	template<typename T>
	Quaternion<T>& operator-=(Quaternion<T>& q0, const Quaternion<T>& q1)
	{
		for (int i = 0; i < 4; i++)
			q0[i] -= q1[i];
		return q0;
	}

	template<typename T>
	Quaternion<T>& operator*=(Quaternion<T>& q, T scalar)
	{
		for (int i = 0; i < 4; i++)
			q[i] *= scalar;
		return q;
	}

	template<typename T>
	Quaternion<T>& operator/=(Quaternion<T>& q, T scalar)
	{
		for (int i = 0; i < 4; i++)
			q[i] /= scalar;
		return q;
	}
}