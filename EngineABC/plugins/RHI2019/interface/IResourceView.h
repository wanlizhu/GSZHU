#pragma once

#include "IDeviceObject.h"
#include <stdint.h>
#include <memory>

namespace ZHU
{
	class IResource;

	enum class EResourceViewType
	{
		Undefined,
		SRV,
		CBV,
		UAV,
		RTV,
		DSV,
	};

	struct RHI_API ResourceViewDesc
	{
		EResourceViewType mType = EResourceViewType::Undefined;
		uint32_t mMostDetailedMip = 0;
		uint32_t mMipCount = UINT_MAX;
		uint32_t mFirstArraySlice = 0;
		uint32_t mArraySize = UINT_MAX;

		ResourceViewDesc();
		ResourceViewDesc(uint32_t mip,
						 uint32_t mipCount,
						 uint32_t firstSlice,
						 uint32_t arraySize);
		bool operator==(const ResourceViewDesc& rhs) const;
		operator bool() const;
	};

	class IResourceView : public IDeviceObject
	{
	public:
		virtual ~IResourceView() = 0;
		virtual const ResourceViewDesc& GetViewDesc() const = 0;
		virtual void Invalidate() = 0;
		virtual IResource* GetResource() const = 0;
	};
}