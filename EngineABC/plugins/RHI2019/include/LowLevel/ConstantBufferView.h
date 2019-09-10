#pragma once

#include "ResourceViewBase.h"

namespace ZHU
{
	class ConstBufferView : public ResourceViewBase<CBVHandle>
	{
	public:
		using SharedPtr = std::shared_ptr<ConstBufferView>;
		using SharedConstPtr = std::shared_ptr<const ConstBufferView>;
		static const SharedPtr kNullView;

		static SharedPtr Create(ResourceWeakPtr res);

	protected:
		ConstBufferView(ResourceWeakPtr res, NativeHandle handle)
			: ResourceViewBase(res, handle, ResourceViewInfo(0, 1, 0, 1))
		{}
	};
}