#pragma once

#include "GMLUtils.h"
#include <initializer_list>
#include <type_traits>
#include <algorithm>
#include <cassert>

namespace GML
{
#ifdef GML_USE_ROW_MAJOR
#define INDEX(r, c) (kNumColumns * r + c)
#else
#define INDEX(r, c) (kNumRows * c + r)
#endif

	struct TensorBase {};
	typedef struct _ColumnIndex {} *ColumnIndex;
	typedef struct _RowIndex {} *RowIndex;

    template<typename T, int H, int W>
    class Tensor : public TensorBase
    {
    public:
		using Element = T;
		static const int  kNumRows    = H;
		static const int  kNumColumns = W;
		static const bool kSquared    = (H == W);
		static const int  kLength     = H * W;

        Tensor() = default;
		template<typename U>       explicit Tensor(const U& uni);
		template<typename U>       explicit Tensor(const U* buffer);
		template<typename U>       explicit Tensor(const std::initializer_list<U>& list);
		template<typename... ARGS> explicit Tensor(const ARGS& ... args);

		inline       T& operator[](int i)       { return mArray[i]; }
		inline const T& operator[](int i) const { return mArray[i]; }
		inline       T& operator()(int r, int c)       { return operator[](INDEX(r, c)); }
		inline const T& operator()(int r, int c) const { return operator[](INDEX(r, c)); }
		inline operator       T* ()       { return mArray; }
		inline operator const T* () const { return mArray; }

		inline       T* begin()        { return mArray; }
		inline const T* cbegin() const { return mArray; }
		inline       T* end()          { return mArray + (H * W); }
		inline const T* cend()   const { return mArray + (H * W); }

		inline bool operator==(const Tensor& other) const { return OP_EQ<T, kLength>(mArray, other.mArray);  }
		inline bool operator!=(const Tensor& other) const { return !operator==(other);   }

	protected:
		T mArray[H * W] = { (T)0 };
	};




	template<typename T, int H, int W>
	template<typename U>
	Tensor<T, H, W>::Tensor(const U& uni) 
	{
		static_assert(kNumRows >= 1 && kNumColumns >= 1, "");
		std::for_each(begin(), end(), [&uni](T* it) { *it = uni; });
	}

	template<typename T, int H, int W>
	template<typename U>
	Tensor<T, H, W>::Tensor(const U* buffer)
	{
		static_assert(kNumRows >= 1 && kNumColumns >= 1, "");
		std::for_each(begin(), end(), [&](T* it) { *it = buffer[it - mArray]; });
	}

	template<typename T, int H, int W>
	template<typename U>
	Tensor<T, H, W>::Tensor(const std::initializer_list<U>& list)
	{
		static_assert(kNumRows >= 1 && kNumColumns >= 1, "");
		assert(list.size() >= (kNumRows * kNumColumns));
		auto list_it = list.begin();
		std::for_each(begin(), end(), [&](T* it) { *it = *list_it; ++list_it; });
	}

	template<typename T, int H, int W>
	template<typename... ARGS>
	Tensor<T, H, W>::Tensor(const ARGS& ... args)
	{
		static_assert((!std::is_base_of_v<TensorBase, const ARGS&> && ...));
		static_assert((std::is_constructible_v<T, const ARGS&> && ...));

		int index = 0;
		auto _push_back = [&](const T& num) { mArray[index++] = num; };
		(_push_back(args), ...);
	}
}