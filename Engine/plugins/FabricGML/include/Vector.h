#pragma once

#include "Tensor.h"

namespace GML
{
#ifdef GML_USE_ROW_MAJOR
#define VECTOR_BASE Tensor<T, 1, N>
#else
#define VECTOR_BASE Tensor<T, N, 1>
#endif

	template<typename T, int N>
	class Vector : public VECTOR_BASE
	{
	public:
		using Base = VECTOR_BASE;

		Vector() : Base() {}
		template<typename U>       explicit Vector(const U& uni) : Base(uni) {}
		template<typename U>       explicit Vector(const U* buffer) : Base(buffer) {}
		template<typename U>       explicit Vector(const std::initializer_list<U>& list) : Base(list) {}
		template<typename... ARGS> explicit Vector(const ARGS& ... args) : Base(std::forward<const ARGS&>(args) ...) {}

		inline       T& operator[](int index)       { return mArray[index]; }
		inline const T& operator[](int index) const { return mArray[index]; }
		
		inline       T& x()       { return mArray[0]; }
		inline const T& x() const { return mArray[0]; }
		inline       T& y()       { return mArray[1]; }
		inline const T& y() const { return mArray[1]; }
		inline       T& z()       { return mArray[2]; }
		inline const T& z() const { return mArray[2]; }
		inline       T& w()       { return mArray[3]; }
		inline const T& w() const { return mArray[3]; }

		inline Vector& make_zero();
		inline Vector& make_one();
		inline Vector& make_unit(int index);

		inline static Vector zero()          { return Vector().make_zero(); }
		inline static Vector one()           { return Vector().make_one(); }
		inline static Vector unit(int index) { return Vector().make_unit(index); }
	};

	template<typename T>
	using Vector2 = Vector<T, 2>;

	template<typename T>
	using Vector3 = Vector<T, 3>;

	template<typename T>
	using Vector4 = Vector<T, 4>;




	template<typename T, int N>
	Vector<T, N>& Vector<T, N>::make_zero()
	{
		for (int i = 0; i < N; i++) mArray[i] = (T)0;
	}

	template<typename T, int N>
	Vector<T, N>& Vector<T, N>::make_one()
	{
		for (int i = 0; i < N; i++) mArray[i] = (T)1;
	}

	template<typename T, int N>
	Vector<T, N>& Vector<T, N>::make_unit(int index)
	{
		for (int i = 0; i < N; i++) mArray[i] = i == index ? (T)1 : (T)0;
	}

	template<typename T, int N>
	Vector<T, N> dot(const Vector<T, N>& v0, const Vector<T, N>& v1)
	{
		T sum = v0[0] * v1[0];
		for (int i = 1; i < N; ++i)
			sum += v0[i] * v1[i];
		return sum;
	}

	template<typename T, int N>
	Vector<T, N> cross(const Vector<T, N> & v0, const Vector<T, N> & v1)
	{
		static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

		Vector<T, N> result;
		result.make_zero();
		result[0] = v0[1] * v1[2] - v0[2] * v1[1];
		result[1] = v0[2] * v1[0] - v0[0] * v1[2];
		result[2] = v0[0] * v1[1] - v0[1] * v1[0];
		return result;
	}

	template<typename T, int N>
	Vector<T, N>& normalize(Vector<T, N>& vec, bool robust = false)
	{
		if (robust)
		{
			T maxAbsComp = std::abs(vec[0]);
			for (int i = 1; i < N; ++i)
			{
				T absComp = std::abs(vec[i]);
				if (absComp > maxAbsComp)
					maxAbsComp = absComp;
			}

			T len;
			if (maxAbsComp > (T)0)
			{
				vec /= maxAbsComp;
				len = std::sqrt(dot(vec, vec));
				vec /= len;
				len *= maxAbsComp;
			}
			else
			{
				len = (T)0;
				for (int i = 0; i < N; ++i)
					vec[i] = (T)0;
			}
			return len;
		}
		else
		{
			T len = std::sqrt(dot(vec, vec));
			if (len > (T)0)
				vec /= len;
			else
			{
				for (int i = 0; i < N; ++i)
					vec[i] = (T)0;
			}
			return len;
		}
	}

	template<typename T, int N>
	T length(const Vector<T, N>& vec, bool robust = false)
	{
		if (robust)
		{
			T maxAbsComp = std::abs(vec[0]);
			for (int i = 1; i < N; ++i)
			{
				T absComp = std::abs(vec[i]);
				if (absComp > maxAbsComp)
					maxAbsComp = absComp;
			}

			if (maxAbsComp > (T)0)
			{
				Vector<T, N> scaled = vec / maxAbsComp;
				return maxAbsComp * std::sqrt(dot(scaled, scaled));
			}
			else return (T)0;
		}
		else return std::sqrt(dot(vec, vec));
	}

	template<typename T, int N>
	T orthonormalize(int num, Vector<T, N>* vecs, bool robust = false)
	{
		if (vecs && 1 <= num && num <= N)
		{
			T minLength = normalize(vecs[0], robust);
			for (int i = 1; i < num; ++i)
			{
				for (int j = 0; j < i; ++j)
				{
					T dotVal = dot(vecs[i], vecs[j]);
					vecs[i] -= vecs[j] * dotVal;
				}
				T len = normalize(vecs[i], robust);
				if (len < minLength)
				{
					minLength = len;
				}
			}
			return minLength;
		}

		return (T)0;
	}

	template<typename T, int N>
	bool compute_bounds(int num, const Vector<T, N>* vecs,
						Vector<T, N>* vmin, Vector<T, N>* vmax)
	{
		if (vecs && num > 0)
		{
			vmin = vecs[0];
			vmax = vmin;
			for (int j = 1; j < num; ++j)
			{
				const Vector<T, N>& vec = vecs[j];
				for (int i = 0; i < N; ++i)
				{
					if (vec[i] < vmin[i])
						vmin[i] = vec[i];
					else if (vec[i] > vmax[i])
						vmax[i] = vec[i];
				}
			}
			return true;
		}

		return false;
	}

	template<typename T>
	inline Vector2<T> perp(const Vector2<T>& vec)
	{
		return Vector2<T>{ vec[1], -vec[0] };
	}

	template<typename T>
	bool compute_barycentrics(const Vector2<T>& p,
							  const Vector2<T>& v0,
							  const Vector2<T>& v1,
							  const Vector2<T>& v2,
							  T bary[3],
							  T epsilon = std::numeric_limits<T>::epsilon())
	{
		// Compute the vectors relative to V2 of the triangle.
		Vector2<T> diff[3] = { v0 - v2, v1 - v2, p - v2 };

		T det = dot(diff[0], perp(diff[1]));
		if (det < -epsilon || det > epsilon)
		{
			T invDet = ((T)1) / det;
			bary[0] = dot(diff[2], perp(diff[1])) * invDet;
			bary[1] = dot(diff[0], perp(diff[2])) * invDet;
			bary[2] = (T)1 - bary[0] - bary[1];
			return true;
		}

		for (int i = 0; i < 3; ++i)
			bary[i] = (T)0;
		
		return false;
	}

	template<typename T>
	bool compute_barycentrics(const Vector3<T>& p,
							  const Vector3<T>& v0,
							  const Vector3<T>& v1,
							  const Vector3<T>& v2,
							  const Vector3<T>& v3,
							  T bary[4],
							  T epsilon = std::numeric_limits<T>::epsilon())
	{
		// Compute the vectors relative to V3 of the tetrahedron.
		Vector3<T> diff[4] = { v0 - v3, v1 - v3, v2 - v3, p - v3 };

		T det = dot(diff[0], cross(diff[1], diff[2]));
		if (det < -epsilon || det > epsilon)
		{
			T invDet = ((T)1) / det;
			bary[0] = dot(diff[3], cross(diff[1], diff[2])) * invDet;
			bary[1] = dot(diff[3], cross(diff[2], diff[0])) * invDet;
			bary[2] = dot(diff[3], cross(diff[0], diff[1])) * invDet;
			bary[3] = (T)1 - bary[0] - bary[1] - bary[2];
			return true;
		}

		for (int i = 0; i < 4; ++i)
			bary[i] = (T)0;
		
		return false;
	}

	// Compute the hypercross product using the formal determinant:
	//   hcross = det{{e0,e1,e2,e3},{x0,x1,x2,x3},{y0,y1,y2,y3},{z0,z1,z2,z3}}
	//   where e0 = (1,0,0,0), e1 = (0,1,0,0), e2 = (0,0,1,0), e3 = (0,0,0,1),
	//   v0 = (x0,x1,x2,x3), v1 = (y0,y1,y2,y3), and v2 = (z0,z1,z2,z3).
	template<typename T, int N>
	Vector4<T> hyper_cross(const Vector4<T> & v0,
						   const Vector4<T> & v1,
						   const Vector4<T> & v2)
	{
		T m01 = v0[0] * v1[1] - v0[1] * v1[0];  // x0*y1 - y0*x1
		T m02 = v0[0] * v1[2] - v0[2] * v1[0];  // x0*z1 - z0*x1
		T m03 = v0[0] * v1[3] - v0[3] * v1[0];  // x0*w1 - w0*x1
		T m12 = v0[1] * v1[2] - v0[2] * v1[1];  // y0*z1 - z0*y1
		T m13 = v0[1] * v1[3] - v0[3] * v1[1];  // y0*w1 - w0*y1
		T m23 = v0[2] * v1[3] - v0[3] * v1[2];  // z0*w1 - w0*z1
		return Vector4<T> {
			+m23 * v2[1] - m13 * v2[2] + m12 * v2[3],  // +m23*y2 - m13*z2 + m12*w2
			-m23 * v2[0] + m03 * v2[2] - m02 * v2[3],  // -m23*x2 + m03*z2 - m02*w2
			+m13 * v2[0] - m03 * v2[1] + m01 * v2[3],  // +m13*x2 - m03*y2 + m01*w2
			-m12 * v2[0] + m02 * v2[1] - m01 * v2[2]   // -m12*x2 + m02*y2 - m01*z2
		};
	}

	//////////////////////////////////////////////////
	///
	/// Vector operators as free function
	///
	//////////////////////////////////////////////////

	template<typename T, int N>
	inline Vector<T, N> operator-(const Vector<T, N>& vec)
	{
		return OP_NEG(vec);
	}

	template<typename T, int N>
	inline Vector<T, N> operator+(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		return OP_PLUS(lhs, rhs);
	}

	template<typename T, int N>
	inline Vector<T, N> operator-(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		return OP_MINUS(lhs, rhs);
	}

	template<typename T, int N>
	inline Vector<T, N> operator*(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		return OP_MUL(lhs, rhs);
	}

	template<typename T, int N>
	inline Vector<T, N> operator/(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		return OP_DIV(lhs, rhs);
	}

	template<typename T, int N>
	inline Vector<T, N> operator*(const Vector<T, N>& lhs, const T& num)
	{
		return OP_MUL(lhs, num);
	}

	template<typename T, int N>
	inline Vector<T, N> operator*(const T& num, const Vector<T, N>& rhs)
	{
		return OP_MUL(num, rhs);
	}

	template<typename T, int N>
	inline Vector<T, N>& operator+=(Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		lhs = OP_PLUS(lhs, rhs);
		return lhs;
	}

	template<typename T, int N>
	inline Vector<T, N>& operator-=(Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		lhs = OP_MINUS(lhs, rhs);
		return lhs;
	}

	template<typename T, int N>
	inline Vector<T, N>& operator*=(Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		lhs = OP_MUL(lhs, rhs);
		return lhs;
	}

	template<typename T, int N>
	inline Vector<T, N>& operator/=(Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{
		lhs = OP_DIV(lhs, rhs);
		return lhs;
	}

	template<typename T, int N>
	inline Vector<T, N>& operator*=(Vector<T, N>& lhs, const T& num)
	{
		lhs = OP_MUL(lhs, num);
		return lhs;
	}
}