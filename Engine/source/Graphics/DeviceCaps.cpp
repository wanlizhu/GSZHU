#include "Graphics/RHI/DeviceCaps.h"
#include <cassert>

namespace ZHU
{
	DeviceCaps::NDCAttribs DeviceCaps::GetNDCAttribs() const
	{
		if (DeviceType == EDevice::D3D11 ||
			DeviceType == EDevice::D3D12)
		{
			static constexpr const NDCAttribs sAttribs{ 0.0f, 1.0f, -0.5f };
			return sAttribs;
		}
		else if (DeviceType == EDevice::Vulkan)
		{
			// Vulkan itself does not invert Y coordinate when transforming
            // normalized device Y to window space. However, we use negative viewport
            // height which achieves the same effect as in D3D, thererfore we need to
            // invert y
			static constexpr const NDCAttribs sAttribs{ 0.0f, 1.0f, -0.5f };
			return sAttribs;
		}
		else if (DeviceType == EDevice::OpenGL)
		{
			static constexpr const NDCAttribs sAttribs{ -1.0f, 0.5f, 0.5f };
			return sAttribs;
		}
		else
		{
			assert(false);
			return {};
		}
	}
}