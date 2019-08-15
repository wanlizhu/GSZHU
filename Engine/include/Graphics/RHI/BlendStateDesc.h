#pragma once

#include "Graphics/RHI/Constants.h"
#include <vector>

namespace ZHU
{
	struct ZHU_GS_API BlendStateDesc
	{
		struct ZHU_GS_API BlendEquation
		{
			EBlendFactor Source = EBlendFactor::One;
			EBlendFactor Destination = EBlendFactor::Zero;
			EBlendOperation Operation = EBlendOperation::Add;
		};

		struct ZHU_GS_API RenderTargetBlend
		{
			bool EnableBlend = false;
			bool EnableLogicOperation = false;
			EColorMask RenderTargetWriteMask = EColorMask::All;

			BlendEquation EquationRGB;
			BlendEquation EquationAlpha;
		};

		bool EnableAlphaToCoverage = false;
		bool EnableIndependentBlend = false;
		std::vector<RenderTargetBlend> RenderTargets;
	};
}