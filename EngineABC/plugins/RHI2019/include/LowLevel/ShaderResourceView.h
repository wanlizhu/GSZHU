#pragma once

#include "ResourceViewBase.h"

namespace ZHU
{
	class ShaderResourceView : public ResourceViewBase<SRVHandle>
	{
	public:
		using SharedPtr = std::shared_ptr<ShaderResourceView>;
		using SharedConstPtr = std::shared_ptr<const ShaderResourceView>;
		static const SharedPtr kNullView;

		static SharedPtr Create(ResourceWeakPtr res, const ResourceViewInfo& info);
		
	protected:
		ShaderResourceView(ResourceWeakPtr res, NativeHandle handle, const ResourceViewInfo& info)
			: ResourceViewBase(res, handle, info)
		{}
	};
}