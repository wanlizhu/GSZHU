#pragma once

#include "Graphics/Config.h"
#include <array>

namespace ZHU
{
	template<typename T>
	struct Box
	{
		struct {
			T min = T(0);
			T max = T(0);
		} x, y, z;

		Box() = default;
		Box(T xmin, T xmax) 
			: x{ xmin, xmax }
		{}
		Box(T xmin, T xmax, T ymin, T ymax)
			: x{ xmin, xmax }
			, y{ ymin, ymax }
		{}
		Box(T xmin, T xmax, T ymin, T ymax, T zmin, T zmax)
			: x{ xmin, xmax }
			, y{ ymin, ymax }
			, z{ zmin, zmax }
		{}

		inline std::array<T, 3> MinPoint() const { return { x.min, y.min, z.min }; }
		inline std::array<T, 3> MaxPoint() const { return { x.max, y.max, z.max }; }
		inline operator bool() const { return operator!=(*this); }
		inline bool operator==(const Box& other) const { return MinPoint() == MaxPoint(); }
		inline bool operator!=(const Box& other) const { return !operator==(other); }
	};
}