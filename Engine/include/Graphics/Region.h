#pragma once

#include <array>

namespace ZHU
{
	template<typename T>
	struct Region
	{
		struct {
			T min = T(0);
			T max = T(0);
		} x, y, z;

		Region() = default;
		Region(T xmin, T xmax) 
			   : x{ xmin, xmax }
		{}
		Region(T xmin, T xmax, T ymin, T ymax)
			   : x{ xmin, xmax }
			   , y{ ymin, ymax }
		{}
		Region(T xmin, T xmax, T ymin, T ymax, T zmin, T zmax)
			   : x{ xmin, xmax }
			   , y{ ymin, ymax }
			   , z{ zmin, zmax }
		{}

		inline std::array<T, 3> MinPoint() const { return { x.min, y.min, z.min }; }
		inline std::array<T, 3> MaxPoint() const { return { x.max, y.max, z.max }; }
		inline operator bool() const { return operator!=(*this); }
		inline bool operator==(const Region& other) const { return MinPoint() == MaxPoint(); }
		inline bool operator!=(const Region& other) const { return !operator==(other); }
	};
}