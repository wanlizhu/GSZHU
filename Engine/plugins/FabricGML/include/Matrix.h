#pragma once

#include "Vector.h"

namespace GML
{
#define COLIDX(i) (reinterpret_cast<ColumnIndex>(i))
#define ROWIDX(i) (reinterpret_cast<RowIndex>   (i))
	
	template<typename T, int H, int W>
	class Matrix : public Tensor<T, H, W>
	{
	public:
		using Base   = Tensor<T, H, W>;
		using Row    = Vector<T, W>;
		using Column = Vector<T, H>;

		Matrix() : Base() {}
		template<typename U>       explicit Matrix(const U& uni) : Base(uni) {}
		template<typename U>       explicit Matrix(const U* buffer) : Base(buffer) {}
		template<typename U>       explicit Matrix(const std::initializer_list<U>& list) : Base(list) {}
		template<typename... ARGS> explicit Matrix(const ARGS& ... args) : Base(std::forward<const ARGS&>(args) ...) {}

		inline const Column& operator[](ColumnIndex index) const;
		inline const    Row& operator[](RowIndex    index) const;
		inline void set(ColumnIndex index, const Column& column);
		inline void set(RowIndex    index, const Row&    row);

		inline Matrix& make_zero();
		inline Matrix& make_identity();
		inline Matrix& make_unit(int r, int c);

		inline static Matrix zero()             { return Matrix().make_zero(); }
		inline static Matrix unit(int r, int c) { return Matrix().make_unit(r, c); }
		inline static Matrix identity()         { return Matrix().make_identity(); }
	};




	template<typename T, int H, int W>
	inline const Matrix<T, H, W>::Column& 
		Matrix<T, H, W>::operator[](ColumnIndex index) const
	{
		int r = 0;
		int c = reinterpret_cast<int>(index);
		assert(c >= 0 && c < W);

		static Column column;
		std::for_each(column.begin(), column.end(), 
					  [&](T* it) { *it = this->operator(r++, c); });
		return column;
	}

	template<typename T, int H, int W>
	inline const Matrix<T, H, W>::Row&
		Matrix<T, H, W>::operator[](RowIndex index) const
	{
		int r = reinterpret_cast<int>(index);
		int c = 0;
		assert(r >= 0 && r < H);

		static Row row;
		std::for_each(row.begin(), row.end(),
					  [&](T* it) { *it = this->operator(r, c++); });
		return row;
	}

	template<typename T, int H, int W>
	inline void Matrix<T, H, W>::set(ColumnIndex index, const Column& column)
	{
		int r = 0;
		int c = reinterpret_cast<int>(index);
		assert(c >= 0 && c < W);

		std::for_each(column.cbegin(), column.cend(), 
					  [&](const T& it) { this->operator(r++, c) = *it; });
	}

	template<typename T, int H, int W>
	inline void Matrix<T, H, W>::set(RowIndex index, const Row& row)
	{
		int r = reinterpret_cast<int>(index);
		int c = 0;
		assert(r >= 0 && r < H);

		std::for_each(row.cbegin(), row.cend(),
					  [&](const T& it) { this->operator(r, c++) = *it; });
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W>& Matrix<T, H, W>::make_zero()
	{
		std::for_each(begin(), end(), [&](T* it) { *it = (T)0; });
		return *this;
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W>& Matrix<T, H, W>::make_identity()
	{
		for (int r = 0; r < H; r++)
			for (int c = 0; c < W; c++)
				operator(r, c) = (r == c ? (T)1 : (T)0);
		return *this;
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W>& Matrix<T, H, W>::make_unit(int r_, int c_)
	{
		for (int r = 0; r < H; r++)
			for (int c = 0; c < W; c++)
				operator(r, c) = ((r == r_ && c == c_) ? (T)1 : (T)0);
		return *this;
	}




	template<typename T, int H, int W>
	Vector<T, H> apply(const Matrix<T, H, W>& mat, const Vector<T, W>& vec)
	{

	}

	template<typename T, int N>
	Matrix<T, N, N> inverse(const Matrix<T, N, N>& mat, bool* invertibility = nullptr)
	{

	}

	template<typename T, int N>
	T determinant(const Matrix<T, N, N>& mat)
	{

	}

	template<typename T, int H, int W>
	Matrix<T, W, H> transpose(const Matrix<T, H, W>& mat)
	{

	}

	// M * M
	template<typename T, int H, int C, int W>
	Matrix<T, H, W> mul(const Matrix<T, H, C>& lhs, const Matrix<T, C, W>& rhs)
	{

	}

	// M * V
	template<typename T, int H, int W>
	Vector<T, H> mul(const Matrix<T, H, W>& mat, const Vector<T, W>& vec)
	{

	}

	//////////////////////////////////////////////////
	///
	/// Matrix2
	///
	//////////////////////////////////////////////////

	template<typename T>
	using Matrix2 = Matrix<T, 2, 2>;

	template<typename T>
	Matrix2<T> inverse(const Matrix2<T>& mat, bool* invertibility = nullptr)
	{

	}

	template<typename T>
	Matrix2<T> adjoint(const Matrix2<T>& mat)
	{

	}

	template<typename T>
	T determinant(const Matrix2<T>& mat)
	{

	}

	// Create a rotation matrix from an angle (in radians).  The matrix is
	// [GTE_USE_MAT_VEC]
	//   R(t) = {{c,-s},{s,c}}
	// [GTE_USE_VEC_MAT]
	//   R(t) = {{c,s},{-s,c}}
	// where c = cos(t), s = sin(t), and the inner-brace pairs are rows of the
	// matrix.
	template<typename T>
	void make_rotation(T angle, Matrix2<T>* mat)
	{

	}

	// Get the angle (radians) from a rotation matrix.  The caller is
	// responsible for ensuring the matrix is a rotation.
	template<typename T>
	T get_angle(const Matrix2<T>& mat)
	{

	}




	//////////////////////////////////////////////////
	///
	/// Matrix3
	///
	//////////////////////////////////////////////////

	template<typename T>
	using Matrix3 = Matrix<T, 3, 3>;

	template<typename T>
	Matrix3<T> inverse(const Matrix3<T>& mat, bool* invertibility = nullptr)
	{

	}

	template<typename T>
	Matrix3<T> adjoint(const Matrix3<T>& mat)
	{

	}

	template<typename T>
	T determinant(const Matrix3<T>& mat)
	{

	}




	//////////////////////////////////////////////////
	///
	/// Matrix4
	///
	//////////////////////////////////////////////////

	template<typename T>
	using Matrix4 = Matrix<T, 4, 4>;

	template<typename T>
	Matrix4<T> inverse(const Matrix4<T>& mat, bool* invertibility = nullptr)
	{

	}

	template<typename T>
	Matrix4<T> adjoint(const Matrix4<T>& mat)
	{

	}

	template<typename T>
	T determinant(const Matrix4<T>& mat)
	{

	}

	template<typename T>
	Matrix4<T> make_lookat(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up)
	{

	}

	template<typename T>
	Matrix4<T> make_persp_proj(T near_, T far_, T fov, T aspect)
	{

	}

	template<typename T>
	Matrix4<T> make_ortho_proj(T width, T height, T depth)
	{

	}

	// The reflection of a point through a plane is
	//     +-                         -+
	// M = | I-2*N*N^T    2*Dot(N,P)*N |
	//     |     0^T            1      |
	//     +-                         -+
	//
	// where P is a point on the plane and N is a unit-length plane normal.
	template<typename T>
	Matrix4<T> make_reflection(const Vector3<T>& point, const Vector3<T>& normal)
	{

	}

	//////////////////////////////////////////////////
	///
	/// Matrix operators as free function
	///
	//////////////////////////////////////////////////

	template<typename T, int H, int W> 
	inline Matrix<T, H, W> operator-(const Matrix<T, H, W>& mat) 
	{
		return OP_NEG(mat);
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W> operator+(const Matrix<T, H, W>& lhs, const Matrix<T, H, W>& rhs)
	{
		return OP_PLUS(lhs, rhs);
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W> operator-(const Matrix<T, H, W>& lhs, const Matrix<T, H, W>& rhs)
	{
		return OP_MINUS(lhs, rhs);
	}

	template<typename T, int H, int C, int W>
	inline Matrix<T, H, W> operator*(const Matrix<T, H, C>& lhs, const Matrix<T, C, W>& rhs)
	{
		return mul(lhs, rhs);
	}

	template<typename T, int H, int W>
	inline Vector<T, H> operator*(const Matrix<T, H, W>& mat, const Vector<T, W>& vec)
	{
		return mul(mat, vec);
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W> operator/(const Matrix<T, H, W>& lhs, const Matrix<T, H, W>& rhs)
	{
		return OP_DIV(lhs, rhs);
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W> operator*(const Matrix<T, H, W>& lhs, const T& num)
	{
		return OP_MUL(lhs, num);
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W> operator*(const T& num, const Matrix<T, H, W>& rhs)
	{
		return OP_MUL(num, rhs);
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W>& operator+=(Matrix<T, H, W>& lhs, const Matrix<T, H, W>& rhs)
	{
		lhs = OP_PLUS(lhs, rhs);
		return lhs;
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W>& operator-=(Matrix<T, H, W>& lhs, const Matrix<T, H, W>& rhs)
	{
		lhs = OP_MINUS(lhs, rhs);
		return lhs;
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W>& operator*=(Matrix<T, H, W>& lhs, const Matrix<T, H, W>& rhs)
	{
		lhs = OP_MUL(lhs, rhs);
		return lhs;
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W>& operator/=(Matrix<T, H, W>& lhs, const Matrix<T, H, W>& rhs)
	{
		lhs = OP_DIV(lhs, rhs);
		return lhs;
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W>& operator*=(Matrix<T, H, W>& lhs, const T& num)
	{
		lhs = OP_MUL(lhs, num);
		return lhs;
	}
}