#pragma once

#include "ICopyContext.h"
#include "IComputeContext.h"
#include "IRenderContext.h"

namespace ZHU
{
	class IDeviceContext : public ICopyContext
		                 , public IComputeContext
		                 , public IRenderContext
	{

	};
}