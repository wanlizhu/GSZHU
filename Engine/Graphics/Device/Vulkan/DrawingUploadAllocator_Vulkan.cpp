#define VMA_IMPLEMENTATION
#include "DrawingUploadAllocator_Vulkan.h"
#include "DrawingDevice_Vulkan.h"
#include "DrawingCommandManager_Vulkan.h"
#include "DrawingRawResource_Vulkan.h"
#include "DrawingUtil_Vulkan.h"

using namespace Engine;

DrawingUploadAllocator_Vulkan::DrawingUploadAllocator_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice)
	: m_pDevice(pDevice)
{
	VmaAllocatorCreateInfo createInfo = {};
	createInfo.physicalDevice = pDevice->m_physicalDevice;
	createInfo.device = pDevice->m_logicalDevice;
	vmaCreateAllocator(&createInfo, &m_allocator);

#if defined(ENABLE_TRANSFER_QUEUE_VK)
	m_pUploadCommandManager = pDevice->GetCommandManager(eCommandList_Copy);
#else
	m_pUploadCommandManager = pDevice->GetCommandManager(eCommandList_Direct);
#endif
}

DrawingUploadAllocator_Vulkan::~DrawingUploadAllocator_Vulkan()
{
	vmaDestroyAllocator(m_allocator);
}

uint32_t DrawingUploadAllocator_Vulkan::FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties) const
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_pDevice->m_physicalDevice, &memProperties);
	
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type.");
	return -1;
}

bool DrawingUploadAllocator_Vulkan::CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags usage, const VmaMemoryUsage memoryUsage, VkBuffer& buffer, VmaAllocation& allocation)
{
	assert(m_allocator != VK_NULL_HANDLE);

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = memoryUsage;

	return VK_CheckResult(vmaCreateBuffer(m_allocator, &bufferInfo, &allocInfo, &buffer, &allocation, nullptr), "vmaCreateBuffer");
}

bool DrawingUploadAllocator_Vulkan::CreateImage(const RawImageCreateInfo_Vulkan& createInfo, DrawingRawImage_Vulkan& rawImage)
{
	assert(m_allocator != VK_NULL_HANDLE);

	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_DetermineImageType(createInfo.extent);
	imageInfo.extent = createInfo.extent;
	imageInfo.mipLevels = createInfo.mipLevels;
	imageInfo.arrayLayers = 1;
	imageInfo.format = createInfo.format;
	imageInfo.tiling = createInfo.tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = createInfo.usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = createInfo.memoryUsage;

	rawImage.m_extent = createInfo.extent;
	rawImage.m_format = createInfo.format;
	rawImage.m_mipLevels = createInfo.mipLevels;

	return VK_CheckResult(vmaCreateImage(m_allocator, &imageInfo, &allocInfo, &rawImage.m_image, &rawImage.m_allocation, nullptr), "vmaCreateImage");
}

bool DrawingUploadAllocator_Vulkan::CopyBuffer_Immediate(const VkBuffer& src, VkBuffer& dst, const VkDeviceSize size)
{
	DrawingCommandBuffer_Vulkan commandBuffer = m_pUploadCommandManager->BeginSingleTimeCommands();

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer.m_bufferHandle, src, dst, 1, &copyRegion);

	return m_pUploadCommandManager->EndSingleTimeCommands(commandBuffer);
}

bool DrawingUploadAllocator_Vulkan::CopyBuffer_Recorded(const std::shared_ptr<DrawingRawBuffer_Vulkan> src, VkBuffer& dst, const VkDeviceSize size)
{
	VkBufferCopy copyRegion = {};
	copyRegion.size = size;

#if defined(ENABLE_TRANSFER_QUEUE_VK)
	assert(m_pUploadCommandManager->IsCommandBufferInUse(m_pDevice->t_recordingTransCmdBufferID));
	return m_pUploadCommandManager->CopyBuffer(m_pDevice->t_recordingTransCmdBufferID, src, dst, copyRegion);
#else
	assert(m_pUploadCommandManager->IsCommandBufferInUse(m_pDevice->t_recordingCmdBufferID));
	return m_pUploadCommandManager->CopyBuffer(m_pDevice->t_recordingCmdBufferID, src, dst, copyRegion);
#endif
}

bool DrawingUploadAllocator_Vulkan::CopyBufferToImage_Immediate(const VkBuffer& buffer, VkImage& image, const std::vector<VkBufferImageCopy>& regions)
{
	DrawingCommandBuffer_Vulkan commandBuffer = m_pUploadCommandManager->BeginSingleTimeCommands();
	vkCmdCopyBufferToImage(commandBuffer.m_bufferHandle, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(regions.size()), regions.data());
	return m_pUploadCommandManager->EndSingleTimeCommands(commandBuffer);
}

bool DrawingUploadAllocator_Vulkan::CopyBufferToImage_Recorded(const std::shared_ptr<DrawingRawBuffer_Vulkan> buffer, VkImage& image, const std::vector<VkBufferImageCopy>& regions)
{
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	assert(m_pUploadCommandManager->IsCommandBufferInUse(m_pDevice->t_recordingTransCmdBufferID));
	return m_pUploadCommandManager->CopyBufferToImage(m_pDevice->t_recordingTransCmdBufferID, buffer, image, regions);
#else
	assert(m_pUploadCommandManager->IsCommandBufferInUse(m_pDevice->t_recordingCmdBufferID));
	return m_pUploadCommandManager->CopyBufferToImage(m_pDevice->t_recordingCmdBufferID, buffer, image, regions);
#endif
}

bool DrawingUploadAllocator_Vulkan::MapMemory(VmaAllocation& allocation, void** mappedData)
{
	return VK_CheckResult(vmaMapMemory(m_allocator, allocation, mappedData), "vmaMapMemory");
}

bool DrawingUploadAllocator_Vulkan::UnmapMemory(VmaAllocation& allocation)
{
	vmaUnmapMemory(m_allocator, allocation);
	return true;
}

bool DrawingUploadAllocator_Vulkan::FreeBuffer(VkBuffer& buffer, VmaAllocation& allocation)
{
	assert(m_allocator != VK_NULL_HANDLE && buffer != VK_NULL_HANDLE && allocation != VK_NULL_HANDLE);
	vmaDestroyBuffer(m_allocator, buffer, allocation);
	return true;
}

bool DrawingUploadAllocator_Vulkan::FreeImage(VkImage& image, VmaAllocation& allocation)
{
	assert(m_allocator != VK_NULL_HANDLE && image != VK_NULL_HANDLE && allocation != VK_NULL_HANDLE);
	vmaDestroyImage(m_allocator, image, allocation);
	return true;
}