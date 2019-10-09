#pragma once

#include "BasicTypes.h"

namespace GML
{
    // Axis-angle representation for N = 3 or N = 4.  When N = 4, the axis
    // must be a vector of the form (x,y,z,0) [affine representation of the
    // 3-tuple direction].
	template<typename T, int N>
	class AxisAngle
	{
        static_assert(N == 3 || N == 4);
	public:
		AxisAngle() = default;
		AxisAngle(const Vector<T, N>& axis_, T angle_)
			: mAxis(axis_), mAngle(angle_)
		{}

		const Vector<T, N>& axis()  const { return mAxis; }
		      Vector<T, N>& axis()        { return mAxis; }
        const T&            angle() const { return mAngle; }
			  T&            angle()       { return mAngle; }

	private:
		Vector<T, N> mAxis = { (T)0 };
		T            mAngle = (T)0;
	};
}