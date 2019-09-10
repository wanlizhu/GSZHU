#pragma once

#include "ResourceViewBase.h"

namespace ZHU
{
	class UnorderedAccessView : public ResourceViewBase<UAVHandle>
	{
	public:
		using SharedPtr = std::shared_ptr<UnorderedAccessView>;
		using SharedConstPtr = std::shared_ptr<const UnorderedAccessView>;
		static const SharedPtr kNullView;

		static SharedPtr Create(ResourceWeakPtr res, const ResourceViewInfo& info);

	protected:
		UnorderedAccessView(ResourceWeakPtr res, NativeHandle handle, const ResourceViewInfo& info)
			: ResourceViewBase(res, handle, info)
		{
			assert(info.mMipCount == 1);
		}
	};
}