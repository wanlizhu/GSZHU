#pragma once

#include "Config.h"
#include <vector>
#include <memory>

namespace ZHU
{
	class Resource;
	using ResourceWeakPtr = std::weak_ptr<Resource>;
	
	struct ResourceViewInfo
	{
		static const uint32_t kMax = -1;
		uint32_t mMostDetailedMip = 0;
		uint32_t mMipCount = kMax;
		uint32_t mFirstArraySlice = 0;
		uint32_t mArraySize = kMax;

		ResourceViewInfo() = default;
		ResourceViewInfo(uint32_t mostDetailedMip,
						 uint32_t mipCount,
						 uint32_t firstArraySlice,
						 uint32_t arraySize)
			: mMostDetailedMip(mostDetailedMip)
			, mMipCount(mipCount)
			, mFirstArraySlice(firstArraySlice)
			, mArraySize(arraySize)
		{}
		bool operator==(const ResourceViewInfo& other) const
		{
			return mMostDetailedMip == other.mMostDetailedMip
				&& mMipCount == other.mMipCount
				&& mFirstArraySlice == other.mFirstArraySlice
				&& mArraySize == other.mArraySize;
		}
	};

	template<typename _NativeHandle>
	class ResourceViewBase
	{
	public:
		static const uint32_t kMax = -1;
		using NativeHandle = _NativeHandle;
		using SharedPtr = std::shared_ptr<ResourceViewBase>;
		using SharedConstPtr = std::shared_ptr<const ResourceViewBase>;

		virtual ~ResourceViewBase() { static_assert(false, "ResourceViewBase missing destructor specialization"); }

		const NativeHandle& GetNativeHandle() const { return mNativeHandle; }
		const ResourceViewInfo& GetViewInfo() const { return mViewInfo; }
		Resource* GetResource() const { return mpResource.lock().get(); }

	protected:
		ResourceViewBase() = default;
		ResourceViewBase(ResourceWeakPtr& res, NativeHandle handle, const ResourceViewInfo& info)
			: mHandle(handle)
			, mpResource(res)
			, mViewInfo(info)
		{}

	protected:
		NativeHandle mNativeHandle;
		ResourceViewInfo mViewInfo;
		ResourceWeakPtr mpResource;
	};

	template<> ResourceViewBase<SRVHandle>::~ResourceViewBase();
	template<> ResourceViewBase<DSVHandle>::~ResourceViewBase();
	template<> ResourceViewBase<UAVHandle>::~ResourceViewBase();
	template<> ResourceViewBase<RTVHandle>::~ResourceViewBase();
	template<> ResourceViewBase<CBVHandle>::~ResourceViewBase();
}