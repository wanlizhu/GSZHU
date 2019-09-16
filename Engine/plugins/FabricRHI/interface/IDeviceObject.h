#pragma once

#include "RHIUtils.h"

namespace ZHU
{
	class IDeviceObject
	{
	public:
		virtual const char* GetName() const = 0;
		virtual void SetName(const char* name) = 0;

	};
}