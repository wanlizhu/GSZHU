#pragma once

#include "Utils/Platform/OS.h"
#include <array>

namespace GS
{
	// A class to extract information about displays
	class GS_API MonitorInfo
	{
	public:
		struct Desc
		{
			std::string Identifier;
			std::array<float, 2> Resolution;
			std::array<float, 2> PhysicalSize;
			float PPI;
			bool IsPrimary;
		};

		static std::vector<Desc> GetMinitorDescList();
		static void PrintMonitorInfo();
	};
}