#pragma once

#include "ResourceViewBase.h"

namespace ZHU
{
	class RenderTargetView : public ResourceViewBase<RTVHandle>
	{
	public:
		using SharedPtr = std::shared_ptr<RenderTargetView>;
		using SharedConstPtr = std::shared_ptr<const RenderTargetView>;
		static const SharedPtr kNullView;

		static SharedPtr Create(ResourceWeakPtr res, const ResourceViewInfo& info);

	protected:
		RenderTargetView(ResourceWeakPtr res, NativeHandle handle, const ResourceViewInfo& info)
			: ResourceViewBase(res, handle, info)
		{
			assert(info.mMipCount == 1);
		}
	};
}