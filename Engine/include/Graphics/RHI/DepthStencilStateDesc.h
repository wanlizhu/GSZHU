#pragma once

#include "Graphics/RHI/Constants.h"

namespace ZHU
{
	struct ZHU_GS_API DepthStencilStateDesc
	{
		struct ZHU_GS_API StencilOperation
		{
			EComparisonFunction StencilFunction = EComparisonFunction::Always;
			EStencilOperation StencilFailOperation = EStencilOperation::Keep;
			EStencilOperation StencilDepthFailOperation = EStencilOperation::Keep;
			EStencilOperation StencilPassOperation = EStencilOperation::Keep;
		};

		bool EnableDepth = true;
		bool EnableDepthWrite = true;
		bool EnableStencil = true;

		EComparisonFunction DepthFunction = EComparisonFunction::Less;
		uint8_t StencilReadMask = 0xff;
		uint8_t StencilWriteMask = 0xff;
		StencilOperation FrontFaceStencil;
		StencilOperation BackFaceStencil;
	};
}