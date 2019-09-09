#pragma once

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

	template<typename HANDLE>
	class IResourceView
	{
	public:
		static const uint32_t kMax = -1;
		using Handle = HANDLE;

		IResourceView() = default;
		IResourceView(ResourceWeakPtr& res, Handle handle, const ResourceViewInfo& info)
			: mHandle(handle)
			, mpResource(res)
			, mViewInfo(info)
		{}
		virtual ~IResourceView() { Destroy(); }

		virtual void Destroy() = 0;
		const Handle& GetHandle() const { return mHandle; }
		const ResourceViewInfo& GetViewInfo() const { return mViewInfo; }
		Resource* GetResource() const { return mpResource.lock().get(); }

	protected:
		Handle mHandle;
		ResourceViewInfo mViewInfo;
		ResourceWeakPtr mpResource;
	};
}