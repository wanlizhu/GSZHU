#pragma once
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

// We are using VMA to manage memory allocation
#include "vk_mem_alloc.h"

namespace Engine
{
	struct RawImageCreateInfo_Vulkan
	{
		VkExtent3D			extent = {0, 0, 1};
		VkFormat			format = VK_FORMAT_UNDEFINED;
		VkImageTiling		tiling = VK_IMAGE_TILING_OPTIMAL; // https://renderdoc.org/vkspec_chunked/chap11.html#VkImageTiling
		uint32_t			mipLevels = 1;
		VkImageAspectFlags	aspect = VK_IMAGE_ASPECT_COLOR_BIT;
		VkImageViewType		viewType = VK_IMAGE_VIEW_TYPE_2D;
		VkImageUsageFlags	usage = 0;
		VmaMemoryUsage		memoryUsage = VMA_MEMORY_USAGE_UNKNOWN;
	};

	class DrawingRawImage_Vulkan;
	class DrawingDevice_Vulkan;
	class DrawingRawBuffer_Vulkan;
	class DrawingCommandManager_Vulkan;
	class DrawingUploadAllocator_Vulkan
	{
	public:
		DrawingUploadAllocator_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice);
		virtual ~DrawingUploadAllocator_Vulkan();

		bool CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags usage, const VmaMemoryUsage memoryUsage, VkBuffer& buffer, VmaAllocation& allocation);
		bool CreateImage(const RawImageCreateInfo_Vulkan& createInfo, DrawingRawImage_Vulkan& rawImage);
		
		bool CopyBuffer_Immediate(const VkBuffer& src, VkBuffer& dst, const VkDeviceSize size);
		bool CopyBufferToImage_Immediate(const VkBuffer& buffer, VkImage& image, const std::vector<VkBufferImageCopy>& regions);

		bool CopyBuffer_Recorded(const std::shared_ptr<DrawingRawBuffer_Vulkan> src, VkBuffer& dst, const VkDeviceSize size);
		bool CopyBufferToImage_Recorded(const std::shared_ptr<DrawingRawBuffer_Vulkan> buffer, VkImage& image, const std::vector<VkBufferImageCopy>& regions);

		bool MapMemory(VmaAllocation& allocation, void** mappedData);
		bool UnmapMemory(VmaAllocation& allocation);

		bool FreeBuffer(VkBuffer& buffer, VmaAllocation& allocation);
		bool FreeImage(VkImage& image, VmaAllocation& allocation);

		uint32_t FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties) const;

	private:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::shared_ptr<DrawingCommandManager_Vulkan> m_pUploadCommandManager;
		VmaAllocator m_allocator;
	};
}