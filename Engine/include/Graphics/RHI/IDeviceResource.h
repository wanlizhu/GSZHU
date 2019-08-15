#pragma once

#include "Graphics/RHI/IDeviceObject.h"

namespace ZHU
{
	struct ZHU_GS_API IDeviceResourceDesc : public IDeviceObjectDesc
	{};


	class ZHU_GS_API IDeviceResource : public IDeviceObject
	{
	public:
		using Pointer = std::shared_ptr<IDeviceResource>;
		using Desc = IDeviceResourceDesc;

		virtual EResourceDimension GetResourceDimension() const = 0;
	};
}