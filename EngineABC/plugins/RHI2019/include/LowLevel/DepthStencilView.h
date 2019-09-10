#pragma once

#include "ResourceViewBase.h"

namespace ZHU
{
	class DepthStencilView : public ResourceViewBase<DSVHandle>
	{
	public:
		using SharedPtr = std::shared_ptr<DepthStencilView>;
		using SharedConstPtr = std::shared_ptr<const DepthStencilView>;
		static const SharedPtr kNullView;

		static SharedPtr Create(ResourceWeakPtr res, const ResourceViewInfo& info);

	protected:
		DepthStencilView(ResourceWeakPtr res, NativeHandle handle, const ResourceViewInfo& info)
			: ResourceViewBase(res, handle, info)
		{
			assert(info.mMipCount == 1);
		}
	};
}