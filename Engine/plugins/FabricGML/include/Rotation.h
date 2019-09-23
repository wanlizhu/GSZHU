#pragma once

#include "Matrix.h"
#include "AxisAngle.h"
#include "EulerAngle.h"
#include "Quaternion.h"
#include <variant>

namespace GML
{
	template<typename T, int N,
		     typename = std::enable_if_t<N == 3 || N == 4>>
	class Rotation
	{
	public:
		enum class ERepresentation
		{
			Matrix = 0,
			Quaternion = 1,
			AxisAngle = 2,
			EulerAngles = 3,
		};

	private:
		ERepresentation mRepresentation = ERepresentation::Matrix;
		std::variant<Matrix<T, N, N>, 
			         AxisAngle<T, N>, 
			         EulerAngles<T>, 
			         Quaternion<T>> mVariable;
	};
}