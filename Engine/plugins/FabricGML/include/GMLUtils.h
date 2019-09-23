#pragma once

#include <algorithm>
#include <numeric>
#include <limits>


namespace GML
{
	template<typename T>
	inline bool OP_EQ(const T& lhs, const T& rhs, 
			          const T& eps = std::numeric_limits<T>::epsilon())
	{
		if constexpr (std::is_floating_point_v<T>)
			return std::abs(lhs - rhs) <= eps;
		else
			return lhs == rhs;
	}

	template<typename T, int N>
	inline bool OP_EQ(const T* lhs, const T* rhs)
	{
		for (int i = 0; i < N; i++)
			if (!OP_EQ<T>(lhs[i], rhs[i]))
				return false;
		return true;
	}

	template<typename T>
	inline bool OP_LT(const T& lhs, const T& rhs)
	{
		return lhs < rhs;
	}

	template<typename T, int N>
	inline bool OP_LT(const T* lhs, const T* rhs)
	{
		for (int i = 0; i < N; i++)
			if (OP_LT<T>(lhs[i], rhs[i]))
				return true;
		return false;
	}

	template<typename TENSOR>
	TENSOR OP_NEG(const TENSOR& tensor)
	{
		using T = typename TENSOR::Element;
		TENSOR res(tensor);
		std::for_each(res.begin(), res.end(), 
					  [](T* it) { *it = -(*it); });
		return res;
	}

	template<typename TENSOR>
	TENSOR OP_PLUS(const TENSOR& lhs, const TENSOR& rhs)
	{
		using T = typename TENSOR::Element;
		TENSOR res;
		int i = 0;
		std::for_each(res.begin(), res.end(), 
					  [&](T* it) { *it = lhs[i] + rhs[i]; ++i; });
		return res;
	}

	template<typename TENSOR>
	TENSOR OP_MINUS(const TENSOR& lhs, const TENSOR& rhs)
	{
		using T = typename TENSOR::Element;
		TENSOR res;
		int i = 0;
		std::for_each(res.begin(), res.end(),
					  [&](T* it) { *it = lhs[i] - rhs[i]; ++i; });
		return res;
	}

	template<typename TENSOR>
	TENSOR OP_MUL(const TENSOR& lhs, const TENSOR& rhs)
	{
		using T = typename TENSOR::Element;
		TENSOR res;
		int i = 0;
		std::for_each(res.begin(), res.end(), 
					  [&](T* it) { *it = lhs[i] * rhs[i]; ++i; });
		return res;
	}

	template<typename TENSOR>
	TENSOR OP_MUL(const TENSOR& lhs, const typename TENSOR::Element& num)
	{
		using T = typename TENSOR::Element;
		TENSOR res;
		int i = 0;
		std::for_each(res.begin(), res.end(),
					  [&](T* it) { *it = lhs[i] * num; ++i; });
		return res;
	}

	template<typename TENSOR>
	TENSOR OP_MUL(const typename TENSOR::Element& num, const TENSOR& rhs)
	{
		using T = typename TENSOR::Element;
		TENSOR res;
		int i = 0;
		std::for_each(res.begin(), res.end(),
					  [&](T* it) { *it = num * rhs[i]; ++i; });
		return res;
	}

	template<typename TENSOR>
	TENSOR OP_DIV(const TENSOR& lhs, const TENSOR& rhs)
	{
		using T = typename TENSOR::Element;
		TENSOR res;
		int i = 0;
		std::for_each(res.begin(), res.end(),
					  [&](T* it) { *it = lhs[i] / rhs[i]; ++i; });
		return res;
	}
}