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
	Vector<T, N> dot(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
	{

	}

	template<typename T>
	Vector<T, 3> cross(const Vector<T, 3> & lhs, const Vector<T, 3> & rhs)
	{

	}

	template<typename T, int N>
	Vector<T, N>& normalize(Vector<T, N>& vec, bool robust = false)
	{

	}

	template<typename T, int N>
	T length(const Vector<T, N>& vec)
	{

	}

	template<typename T, int N>
	T orthonormalize(int num, Vector<T, N>* vecs, bool robust = false)
	{

	}

	template<typename T, int N>
	bool compute_bounds(int num, const Vector<T, N>* vecs,
						Vector<T, N>* vmin, Vector<T, N>* vmax)
	{

	}

	template<typename T>
	bool compute_barycentrics(const Vector<T, 3> & p,
							  const Vector<T, 3> & v0,
							  const Vector<T, 3> & v1,
							  const Vector<T, 3> & v2,
							  T bary[4],
							  T epsilon = std::numeric_limits<T>::epsilon())
	{

	}

	// Compute the hypercross product using the formal determinant:
	//   hcross = det{{e0,e1,e2,e3},{x0,x1,x2,x3},{y0,y1,y2,y3},{z0,z1,z2,z3}}
	//   where e0 = (1,0,0,0), e1 = (0,1,0,0), e2 = (0,0,1,0), e3 = (0,0,0,1),
	//   v0 = (x0,x1,x2,x3), v1 = (y0,y1,y2,y3), and v2 = (z0,z1,z2,z3).
	template<typename T, int N>
	Vector<T, 4> hyper_cross(const Vector<T, 4> & v0,
							 const Vector<T, 4> & v1,
							 const Vector<T, 4> & v2)
	{

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