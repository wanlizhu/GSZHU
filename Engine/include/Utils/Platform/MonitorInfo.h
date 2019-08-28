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
			std::wstring Identifier;
			std::array<float, 2> Resolution = { 0.0f };
			std::array<float, 2> PhysicalSize = { 0.0f };
			float PPI = 1.0f;
			bool IsPrimary = true;
		};

		static std::vector<Desc> GetMinitorDescList();
		static void PrintMonitorInfo();
	};
}