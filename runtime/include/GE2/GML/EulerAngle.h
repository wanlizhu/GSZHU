#pragma once

#include "BasicTypes.h"

namespace GML
{
	template <typename T>
	class EulerAngles
	{
	public:
		EulerAngles() = default;
		EulerAngles(T x, T y, T z) 
            : mAngles{ x, y, z }
        {}

		inline const T& operator[](int i) const { return mAngles[i]; }
		inline       T& operator[](int i)       { return mAngles[i]; }
        inline const T& x()               const { return mAngles[0]; };
		inline 	     T& x()                     { return mAngles[0]; };
	    inline const T& y()               const { return mAngles[1]; };
		inline 	     T& y()                     { return mAngles[1]; };
		inline const T& z()               const { return mAngles[2]; };
		inline       T& z()                     { return mAngles[2]; };

		inline bool operator==(const EulerAngles& other) const { return OP_EQ<T, 3>(mAngles.data(), other.mAngles.data()); }
		inline bool operator!=(const EulerAngles& other) const { return !operator==(other); }

	private:
		std::array<T, 3> mAngles = { T(0) };
	};
}