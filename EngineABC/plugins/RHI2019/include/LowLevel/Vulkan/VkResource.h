#pragma once

#include "VkHandle.h"
#include <assert.h>

namespace ZHU
{
	enum class VkResourceType
	{
		None,
		Buffer,
		Image,
	};

	template<typename BufferType, typename ImageType>
	class VkResource : public VkHandleBase
		             , public inherit_shared_from_this<VkHandleBase, VkResource<BufferType, ImageType>>
	{
	public:
		class SharedPtr : public std::shared_ptr<VkResource<BufferType, ImageType>>
		{
		public:
			SharedPtr() = default;
			SharedPtr(VkResource<BufferType, ImageType>* res) : std::shared_ptr<VkResource<BufferType, ImageType>>(res) {}

			static SharedPtr Create(BufferType buffer, VkDeviceMemory mem) { return SharedPtr(new VkResource(buffer, mem)); }
			static SharedPtr Create(ImageType image, VkDeviceMemory mem) { return SharedPtr(new VkResource(image, mem)); }
			VkResourceType GetResourceType() const { return get()->mType; }
			operator BufferType() const { assert(get()->mType == VkResourceType::Buffer); return get()->mBufferHandle; }
			operator ImageType() const { assert(get()->mType == VkResourceType::Image); return get()->mImageHandle; }
			operator VkDeviceMemory() const { return get()->mDeviceMemory; }
		};
		friend class SharedPtr;

		virtual ~VkResource() { static_assert(false, "VkResource missing destructor specialization"); }

	private:
		VkResource(BufferType image, VkDeviceMemory mem) : mType(VkResourceType::Buffer), mBufferHandle(image), mDeviceMemory(mem) {}
		VkResource(ImageType buffer, VkDeviceMemory mem) : mType(VkResourceType::Image), mImageHandle(buffer), mDeviceMemory(mem) {}

	private:
		VkResourceType mType = VkResourceType::None;
		BufferType mBufferHandle = VK_NULL_HANDLE;
		ImageType mImageHandle = VK_NULL_HANDLE;
		VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;
	};

	template<> VkResource<VkBuffer, VkImage>::~VkResource();
	template<> VkResource<VkBufferView, VkImageView>::~VkResource();
}