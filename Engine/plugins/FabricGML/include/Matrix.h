#pragma once

#include "Vector.h"

namespace GML
{
#define COLIDX(i) (reinterpret_cast<ColumnIndex>(i))
#define ROWIDX(i) (reinterpret_cast<RowIndex>   (i))
	
	template<typename T, int NumRows, int NumCols>
	class Matrix : public Tensor<T, NumRows, NumCols>
	{
	public:
		using Base   = Tensor<T, NumRows, NumCols>;
		using Row    = Vector<T, NumCols>;
		using Column = Vector<T, NumRows>;

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

	template<typename T>
	using Matrix2 = Matrix<T, 2, 2>;

	template<typename T>
	using Matrix3 = Matrix<T, 3, 3>;

	template<typename T>
	using Matrix4 = Matrix<T, 4, 4>;




	template<typename T, int NumRows, int NumCols>
	inline const Matrix<T, NumRows, NumCols>::Column& 
		Matrix<T, NumRows, NumCols>::operator[](ColumnIndex index) const
	{
		int r = 0;
		int c = reinterpret_cast<int>(index);
		assert(c >= 0 && c < NumCols);

		static Column column;
		std::for_each(column.begin(), column.end(), 
					  [&](T* it) { *it = this->operator(r++, c); });
		return column;
	}

	template<typename T, int NumRows, int NumCols>
	inline const Matrix<T, NumRows, NumCols>::Row&
		Matrix<T, NumRows, NumCols>::operator[](RowIndex index) const
	{
		int r = reinterpret_cast<int>(index);
		int c = 0;
		assert(r >= 0 && r < NumRows);

		static Row row;
		std::for_each(row.begin(), row.end(),
					  [&](T* it) { *it = this->operator(r, c++); });
		return row;
	}

	template<typename T, int NumRows, int NumCols>
	inline void Matrix<T, NumRows, NumCols>::set(ColumnIndex index, const Column& column)
	{
		int r = 0;
		int c = reinterpret_cast<int>(index);
		assert(c >= 0 && c < NumCols);

		std::for_each(column.cbegin(), column.cend(), 
					  [&](const T& it) { this->operator(r++, c) = *it; });
	}

	template<typename T, int NumRows, int NumCols>
	inline void Matrix<T, NumRows, NumCols>::set(RowIndex index, const Row& row)
	{
		int r = reinterpret_cast<int>(index);
		int c = 0;
		assert(r >= 0 && r < NumRows);

		std::for_each(row.cbegin(), row.cend(),
					  [&](const T& it) { this->operator(r, c++) = *it; });
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols>& Matrix<T, NumRows, NumCols>::make_zero()
	{
		std::for_each(begin(), end(), [&](T* it) { *it = (T)0; });
		return *this;
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols>& Matrix<T, NumRows, NumCols>::make_identity()
	{
		for (int r = 0; r < NumRows; r++)
			for (int c = 0; c < NumCols; c++)
				operator(r, c) = (r == c ? (T)1 : (T)0);
		return *this;
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols>& Matrix<T, NumRows, NumCols>::make_unit(int r_, int c_)
	{
		for (int r = 0; r < NumRows; r++)
			for (int c = 0; c < NumCols; c++)
				operator(r, c) = ((r == r_ && c == c_) ? (T)1 : (T)0);
		return *this;
	}





	template<typename T, int N>
	Matrix<T, N, N> inverse(const Matrix<T, N, N>& mat, bool* invertibility = nullptr)
	{
		Matrix<T, N, N> invM;
		T det;
		bool invertible = GaussianElimination<T>()(N,
												   &mat[0], &invM[0], &det,
												   nullptr, nullptr,
												   nullptr, 0, nullptr);
		if (invertibility)
		{
			*invertibility = invertible;
		}
		return invM;
	}

	template<typename T, int N>
	T determinant(const Matrix<T, N, N>& mat)
	{
		T det;
		GaussianElimination<T>()(N, 
								 &mat[0], nullptr, &det,
								 nullptr, nullptr,
								 nullptr, 0, nullptr);
		return det;
	}

	template<typename T, int NumRows, int NumCols>
	Matrix<T, NumCols, NumRows> transpose(const Matrix<T, NumRows, NumCols>& mat)
	{
		Matrix<T, NumCols, NumRows> result;
		for (int r = 0; r < NumRows; ++r)
		{
			for (int c = 0; c < NumCols; ++c)
			{
				result(c, r) = mat(r, c);
			}
		}
		return result;
	}

	// M * M
	template<typename T, int NumRows, int Common, int NumCols>
	Matrix<T, NumRows, NumCols> mul(const Matrix<T, NumRows, Common>& A, const Matrix<T, Common, NumCols>& B)
	{
		Matrix<T, NumRows, NumCols> result;
		for (int r = 0; r < NumRows; ++r)
		{
			for (int c = 0; c < NumCols; ++c)
			{
				result(r, c) = (T)0;
				for (int i = 0; i < Common; ++i)
				{
					result(r, c) += A(r, i) * B(i, c);
				}
			}
		}
		return result;
	}

	// M * V
	template<typename T, int NumRows, int NumCols>
	Vector<T, NumRows> mul(const Matrix<T, NumRows, NumCols>& mat, const Vector<T, NumCols>& vec)
	{
		Vector<T, NumRows> result;
		for (int r = 0; r < NumRows; ++r)
		{
			result[r] = (T)0;
			for (int c = 0; c < NumCols; ++c)
			{
				result[r] += mat(r, c) * vec[c];
			}
		}
		return result;
	}

	template <typename T, int NumRows, int NumCols>
	Matrix<T, NumRows, NumCols> outer_product(const Vector<T, NumRows>& U, const Vector<T, NumCols>& V)
	{
		Matrix<T, NumRows, NumCols> result;
		for (int r = 0; r < NumRows; ++r)
		{
			for (int c = 0; c < NumCols; ++c)
			{
				result(r, c) = U[r] * V[c];
			}
		}
		return result;
	}

	//////////////////////////////////////////////////
	///
	/// Matrix2
	///
	//////////////////////////////////////////////////

	template<typename T>
	Matrix2<T> inverse(const Matrix2<T>& mat, bool* invertibility = nullptr)
	{
		Matrix2<T> inv;
		bool invertible;
		T det = mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);
		if (det != (T)0)
		{
			T invDet = ((T)1) / det;
			inv = Matrix2<T> {
				 mat(1, 1) * invDet, 
				-mat(0, 1) * invDet,
				-mat(1, 0) * invDet, 
				 mat(0, 0) * invDet
			};
			invertible = true;
		}
		else
		{
			inv.make_zero();
			invertible = false;
		}

		if (invertibility)
			*invertibility = invertible;
		
		return inv;
	}

	template<typename T>
	inline Matrix2<T> adjoint(const Matrix2<T>& mat)
	{
		return Matrix2<T> {
			mat(1, 1), -mat(0, 1),
		   -mat(1, 0),  mat(0, 0)
		};
	}

	template<typename T>
	inline T determinant(const Matrix2<T>& mat)
	{
		return mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0)
	}

	// Create a rotation matrix from an angle (in radians).  The matrix is
	// [GTE_USE_MAT_VEC]
	//   R(t) = {{c,-s},{s,c}}
	// [GTE_USE_VEC_MAT]
	//   R(t) = {{c,s},{-s,c}}
	// where c = cos(t), s = sin(t), and the inner-brace pairs are rows of the
	// matrix.
	template<typename T>
	inline void make_rotation(T angle, Matrix2<T>* mat)
	{
		T cs = std::cos(angle);
		T sn = std::sin(angle);
#if defined(GML_USE_ROW_MAJOR)
		(*mat)(0, 0) = cs;
		(*mat)(0, 1) = sn;
		(*mat)(1, 0) = -sn;
		(*mat)(1, 1) = cs;
#else
		(*mat)(0, 0) = cs;
		(*mat)(0, 1) = -sn;
		(*mat)(1, 0) = sn;
		(*mat)(1, 1) = cs;
#endif
	}

	// Get the angle (radians) from a rotation matrix.  The caller is
	// responsible for ensuring the matrix is a rotation.
	template<typename T>
	T get_angle(const Matrix2<T>& mat)
	{
#if defined(GML_USE_ROW_MAJOR)
		return std::atan2(mat(0, 1), mat(0, 0));
#else
		return std::atan2(mat(1, 0), mat(0, 0));
#endif
	}




	//////////////////////////////////////////////////
	///
	/// Matrix3
	///
	//////////////////////////////////////////////////

	template<typename T>
	Matrix3<T> inverse(const Matrix3<T>& mat, bool* invertibility = nullptr)
	{
		Matrix3<T> inv;
		bool invertible;
		T c00 = mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1);
		T c10 = mat(1, 2) * mat(2, 0) - mat(1, 0) * mat(2, 2);
		T c20 = mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0);
		T det = mat(0, 0) * c00 + mat(0, 1) * c10 + mat(0, 2) * c20;
		if (det != (T)0)
		{
			T invDet = (T)1 / det;
			inv = Matrix3<T> {
				c00 * invDet,
				(mat(0, 2) * mat(2, 1) - mat(0, 1) * mat(2, 2)) * invDet,
				(mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1)) * invDet,
				c10 * invDet,
				(mat(0, 0) * mat(2, 2) - mat(0, 2) * mat(2, 0)) * invDet,
				(mat(0, 2) * mat(1, 0) - mat(0, 0) * mat(1, 2)) * invDet,
				c20 * invDet,
				(mat(0, 1) * mat(2, 0) - mat(0, 0) * mat(2, 1)) * invDet,
				(mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0)) * invDet
			};
			invertible = true;
		}
		else
		{
			inv.make_zero();
			invertible = false;
		}

		if (invertibility)
			*invertibility = invertible;
		
		return inv;
	}

	template<typename T>
	Matrix3<T> adjoint(const Matrix3<T>& mat)
	{
		return Matrix3<T> {
			mat(1, 1)* mat(2, 2) - mat(1, 2) * mat(2, 1),
			mat(0, 2)* mat(2, 1) - mat(0, 1) * mat(2, 2),
			mat(0, 1)* mat(1, 2) - mat(0, 2) * mat(1, 1),
			mat(1, 2)* mat(2, 0) - mat(1, 0) * mat(2, 2),
			mat(0, 0)* mat(2, 2) - mat(0, 2) * mat(2, 0),
			mat(0, 2)* mat(1, 0) - mat(0, 0) * mat(1, 2),
			mat(1, 0)* mat(2, 1) - mat(1, 1) * mat(2, 0),
			mat(0, 1)* mat(2, 0) - mat(0, 0) * mat(2, 1),
			mat(0, 0)* mat(1, 1) - mat(0, 1) * mat(1, 0)
		};
	}

	template<typename T>
	T determinant(const Matrix3<T>& mat)
	{
		T c00 = mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1);
		T c10 = mat(1, 2) * mat(2, 0) - mat(1, 0) * mat(2, 2);
		T c20 = mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0);
		T det = mat(0, 0) * c00 + mat(0, 1) * c10 + mat(0, 2) * c20;
		return det;
	}




	//////////////////////////////////////////////////
	///
	/// Matrix4
	///
	//////////////////////////////////////////////////

	template<typename T>
	Matrix4<T> inverse(const Matrix4<T>& mat, bool* invertibility = nullptr)
	{
		Matrix4<T> inv;
		bool invertible;
		T a0 = mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);
		T a1 = mat(0, 0) * mat(1, 2) - mat(0, 2) * mat(1, 0);
		T a2 = mat(0, 0) * mat(1, 3) - mat(0, 3) * mat(1, 0);
		T a3 = mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1);
		T a4 = mat(0, 1) * mat(1, 3) - mat(0, 3) * mat(1, 1);
		T a5 = mat(0, 2) * mat(1, 3) - mat(0, 3) * mat(1, 2);
		T b0 = mat(2, 0) * mat(3, 1) - mat(2, 1) * mat(3, 0);
		T b1 = mat(2, 0) * mat(3, 2) - mat(2, 2) * mat(3, 0);
		T b2 = mat(2, 0) * mat(3, 3) - mat(2, 3) * mat(3, 0);
		T b3 = mat(2, 1) * mat(3, 2) - mat(2, 2) * mat(3, 1);
		T b4 = mat(2, 1) * mat(3, 3) - mat(2, 3) * mat(3, 1);
		T b5 = mat(2, 2) * mat(3, 3) - mat(2, 3) * mat(3, 2);
		T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

		if (det != (T)0)
		{
			T invDet = (T)1 / det;
			inv = Matrix4<T> {
				(+mat(1, 1) * b5 - mat(1, 2) * b4 + mat(1, 3) * b3) * invDet,
				(-mat(0, 1) * b5 + mat(0, 2) * b4 - mat(0, 3) * b3) * invDet,
				(+mat(3, 1) * a5 - mat(3, 2) * a4 + mat(3, 3) * a3) * invDet,
				(-mat(2, 1) * a5 + mat(2, 2) * a4 - mat(2, 3) * a3) * invDet,
				(-mat(1, 0) * b5 + mat(1, 2) * b2 - mat(1, 3) * b1) * invDet,
				(+mat(0, 0) * b5 - mat(0, 2) * b2 + mat(0, 3) * b1) * invDet,
				(-mat(3, 0) * a5 + mat(3, 2) * a2 - mat(3, 3) * a1) * invDet,
				(+mat(2, 0) * a5 - mat(2, 2) * a2 + mat(2, 3) * a1) * invDet,
				(+mat(1, 0) * b4 - mat(1, 1) * b2 + mat(1, 3) * b0) * invDet,
				(-mat(0, 0) * b4 + mat(0, 1) * b2 - mat(0, 3) * b0) * invDet,
				(+mat(3, 0) * a4 - mat(3, 1) * a2 + mat(3, 3) * a0) * invDet,
				(-mat(2, 0) * a4 + mat(2, 1) * a2 - mat(2, 3) * a0) * invDet,
				(-mat(1, 0) * b3 + mat(1, 1) * b1 - mat(1, 2) * b0) * invDet,
				(+mat(0, 0) * b3 - mat(0, 1) * b1 + mat(0, 2) * b0) * invDet,
				(-mat(3, 0) * a3 + mat(3, 1) * a1 - mat(3, 2) * a0) * invDet,
				(+mat(2, 0) * a3 - mat(2, 1) * a1 + mat(2, 2) * a0) * invDet
			};
			invertible = true;
		}
		else
		{
			inv.make_zero();
			invertible = false;
		}

		if (invertibility)
			*invertibility = invertible;
		
		return inv;
	}

	template<typename T>
	Matrix4<T> adjoint(const Matrix4<T>& mat)
	{
		T a0 = mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);
		T a1 = mat(0, 0) * mat(1, 2) - mat(0, 2) * mat(1, 0);
		T a2 = mat(0, 0) * mat(1, 3) - mat(0, 3) * mat(1, 0);
		T a3 = mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1);
		T a4 = mat(0, 1) * mat(1, 3) - mat(0, 3) * mat(1, 1);
		T a5 = mat(0, 2) * mat(1, 3) - mat(0, 3) * mat(1, 2);
		T b0 = mat(2, 0) * mat(3, 1) - mat(2, 1) * mat(3, 0);
		T b1 = mat(2, 0) * mat(3, 2) - mat(2, 2) * mat(3, 0);
		T b2 = mat(2, 0) * mat(3, 3) - mat(2, 3) * mat(3, 0);
		T b3 = mat(2, 1) * mat(3, 2) - mat(2, 2) * mat(3, 1);
		T b4 = mat(2, 1) * mat(3, 3) - mat(2, 3) * mat(3, 1);
		T b5 = mat(2, 2) * mat(3, 3) - mat(2, 3) * mat(3, 2);

		return Matrix4<T> {
			+mat(1, 1) * b5 - mat(1, 2) * b4 + mat(1, 3) * b3,
			-mat(0, 1) * b5 + mat(0, 2) * b4 - mat(0, 3) * b3,
			+mat(3, 1) * a5 - mat(3, 2) * a4 + mat(3, 3) * a3,
			-mat(2, 1) * a5 + mat(2, 2) * a4 - mat(2, 3) * a3,
			-mat(1, 0) * b5 + mat(1, 2) * b2 - mat(1, 3) * b1,
			+mat(0, 0) * b5 - mat(0, 2) * b2 + mat(0, 3) * b1,
			-mat(3, 0) * a5 + mat(3, 2) * a2 - mat(3, 3) * a1,
			+mat(2, 0) * a5 - mat(2, 2) * a2 + mat(2, 3) * a1,
			+mat(1, 0) * b4 - mat(1, 1) * b2 + mat(1, 3) * b0,
			-mat(0, 0) * b4 + mat(0, 1) * b2 - mat(0, 3) * b0,
			+mat(3, 0) * a4 - mat(3, 1) * a2 + mat(3, 3) * a0,
			-mat(2, 0) * a4 + mat(2, 1) * a2 - mat(2, 3) * a0,
			-mat(1, 0) * b3 + mat(1, 1) * b1 - mat(1, 2) * b0,
			+mat(0, 0) * b3 - mat(0, 1) * b1 + mat(0, 2) * b0,
			-mat(3, 0) * a3 + mat(3, 1) * a1 - mat(3, 2) * a0,
			+mat(2, 0) * a3 - mat(2, 1) * a1 + mat(2, 2) * a0
		};
	}

	template<typename T>
	T determinant(const Matrix4<T>& mat)
	{
		T a0 = mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);
		T a1 = mat(0, 0) * mat(1, 2) - mat(0, 2) * mat(1, 0);
		T a2 = mat(0, 0) * mat(1, 3) - mat(0, 3) * mat(1, 0);
		T a3 = mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1);
		T a4 = mat(0, 1) * mat(1, 3) - mat(0, 3) * mat(1, 1);
		T a5 = mat(0, 2) * mat(1, 3) - mat(0, 3) * mat(1, 2);
		T b0 = mat(2, 0) * mat(3, 1) - mat(2, 1) * mat(3, 0);
		T b1 = mat(2, 0) * mat(3, 2) - mat(2, 2) * mat(3, 0);
		T b2 = mat(2, 0) * mat(3, 3) - mat(2, 3) * mat(3, 0);
		T b3 = mat(2, 1) * mat(3, 2) - mat(2, 2) * mat(3, 1);
		T b4 = mat(2, 1) * mat(3, 3) - mat(2, 3) * mat(3, 1);
		T b5 = mat(2, 2) * mat(3, 3) - mat(2, 3) * mat(3, 2);
		T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
		return det;
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
		Matrix4<T> mat;

		const T zero = (T)0;
		const T one  = (T)1;
		const T two  = (T)2;
		T twoDotNO = two * dot(point, normal);

#if defined(GML_USE_ROW_MAJOR)
		mat(0, 0) = one - two * normal[0] * normal[0];
		mat(1, 0) = -two * normal[0] * normal[1];
		mat(2, 0) = -two * normal[0] * normal[2];
		mat(3, 0) = twoDotNO * normal[0];
		mat(0, 1) = mat(1, 0);
		mat(1, 1) = one - two * normal[1] * normal[1];
		mat(2, 1) = -two * normal[1] * normal[2];
		mat(3, 1) = twoDotNO * normal[1];
		mat(0, 2) = mat(2, 0);
		mat(1, 2) = mat(2, 1);
		mat(2, 2) = one - two * normal[2] * normal[2];
		mat(3, 2) = twoDotNO * normal[2];
		mat(0, 3) = zero;
		mat(1, 3) = zero;
		mat(2, 3) = zero;
		mat(3, 3) = one;
#else
		mat(0, 0) = one - two * normal[0] * normal[0];
		mat(0, 1) = -two * normal[0] * normal[1];
		mat(0, 2) = -two * normal[0] * normal[2];
		mat(0, 3) = twoDotNO * normal[0];
		mat(1, 0) = mat(0, 1);
		mat(1, 1) = one - two * normal[1] * normal[1];
		mat(1, 2) = -two * normal[1] * normal[2];
		mat(1, 3) = twoDotNO * normal[1];
		mat(2, 0) = mat(0, 2);
		mat(2, 1) = mat(1, 2);
		mat(2, 2) = one - two * normal[2] * normal[2];
		mat(2, 3) = twoDotNO * normal[2];
		mat(3, 0) = zero;
		mat(3, 1) = zero;
		mat(3, 2) = zero;
		mat(3, 3) = one;
#endif
		return mat;
	}

	//////////////////////////////////////////////////
	///
	/// Matrix operators as free function
	///
	//////////////////////////////////////////////////

	template<typename T, int NumRows, int NumCols> 
	inline Matrix<T, NumRows, NumCols> operator-(const Matrix<T, NumRows, NumCols>& mat) 
	{
		return OP_NEG(mat);
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols> operator+(const Matrix<T, NumRows, NumCols>& lhs, const Matrix<T, NumRows, NumCols>& rhs)
	{
		return OP_PLUS(lhs, rhs);
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols> operator-(const Matrix<T, NumRows, NumCols>& lhs, const Matrix<T, NumRows, NumCols>& rhs)
	{
		return OP_MINUS(lhs, rhs);
	}

	template<typename T, int NumRows, int Common, int NumCols>
	inline Matrix<T, NumRows, NumCols> operator*(const Matrix<T, NumRows, Common>& lhs, const Matrix<T, Common, NumCols>& rhs)
	{
		return mul(lhs, rhs);
	}

	template<typename T, int NumRows, int NumCols>
	inline Vector<T, NumRows> operator*(const Matrix<T, NumRows, NumCols>& mat, const Vector<T, NumCols>& vec)
	{
		return mul(mat, vec);
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols> operator/(const Matrix<T, NumRows, NumCols>& lhs, const Matrix<T, NumRows, NumCols>& rhs)
	{
		return OP_DIV(lhs, rhs);
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols> operator*(const Matrix<T, NumRows, NumCols>& lhs, const T& num)
	{
		return OP_MUL(lhs, num);
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols> operator*(const T& num, const Matrix<T, NumRows, NumCols>& rhs)
	{
		return OP_MUL(num, rhs);
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols>& operator+=(Matrix<T, NumRows, NumCols>& lhs, const Matrix<T, NumRows, NumCols>& rhs)
	{
		lhs = OP_PLUS(lhs, rhs);
		return lhs;
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols>& operator-=(Matrix<T, NumRows, NumCols>& lhs, const Matrix<T, NumRows, NumCols>& rhs)
	{
		lhs = OP_MINUS(lhs, rhs);
		return lhs;
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols>& operator*=(Matrix<T, NumRows, NumCols>& lhs, const Matrix<T, NumRows, NumCols>& rhs)
	{
		lhs = OP_MUL(lhs, rhs);
		return lhs;
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols>& operator/=(Matrix<T, NumRows, NumCols>& lhs, const Matrix<T, NumRows, NumCols>& rhs)
	{
		lhs = OP_DIV(lhs, rhs);
		return lhs;
	}

	template<typename T, int NumRows, int NumCols>
	inline Matrix<T, NumRows, NumCols>& operator*=(Matrix<T, NumRows, NumCols>& lhs, const T& num)
	{
		lhs = OP_MUL(lhs, num);
		return lhs;
	}
}