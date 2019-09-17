#pragma once

#include <algorithm>

#ifdef GML_EXPORT
#define GML_API 
#elif defined(GML_USE_STATIC)
#define GML_API
#else
#define GML_API
#endif

namespace GML
{
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