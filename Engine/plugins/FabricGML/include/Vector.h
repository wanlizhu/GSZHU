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
	class GML_API Vector : public VECTOR_BASE
	{
	public:
		using Base = VECTOR_BASE;

	};
}