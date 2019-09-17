#pragma once

#include "Vector.h"

namespace GML
{
#define COLIDX(i) (reinterpret_cast<ColumnIndex>(i))
#define ROWIDX(i) (reinterpret_cast<RowIndex>   (i))
	
	template<typename T, int H, int W>
	class GML_API Matrix : public Tensor<T, H, W>
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
		inline void Set(ColumnIndex index, const Column& column);
		inline void Set(RowIndex    index, const Row&    row);

		inline Matrix& MakeZero();
		inline Matrix& MakeIdentity();
		inline Matrix& MakeUnit(int r, int c);
	};
	GML_COMPONENT_WISE_OPERATORS("<typename T, int H, int W>", "Matrix<T, H, W>")




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
	inline void Matrix<T, H, W>::Set(ColumnIndex index, const Column& column)
	{
		int r = 0;
		int c = reinterpret_cast<int>(index);
		assert(c >= 0 && c < W);

		std::for_each(column.cbegin(), column.cend(), 
					  [&](const T& it) { this->operator(r++, c) = *it; });
	}

	template<typename T, int H, int W>
	inline void Matrix<T, H, W>::Set(RowIndex index, const Row& row)
	{
		int r = reinterpret_cast<int>(index);
		int c = 0;
		assert(r >= 0 && r < H);

		std::for_each(row.cbegin(), row.cend(),
					  [&](const T& it) { this->operator(r, c++) = *it; });
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W>& Matrix<T, H, W>::MakeZero()
	{
		std::for_each(begin(), end(), [&](T* it) { *it = (T)0; });
		return *this;
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W>& Matrix<T, H, W>::MakeIdentity()
	{
		for (int r = 0; r < H; r++)
			for (int c = 0; c < W; c++)
				operator(r, c) = (r == c ? (T)1 : (T)0);
		return *this;
	}

	template<typename T, int H, int W>
	inline Matrix<T, H, W>& Matrix<T, H, W>::MakeUnit(int r_, int c_)
	{
		for (int r = 0; r < H; r++)
			for (int c = 0; c < W; c++)
				operator(r, c) = ((r == r_ && c == c_) ? (T)1 : (T)0);
		return *this;
	}
}