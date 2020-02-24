#include "DrawingDescriptorAllocator_Vulkan.h"
#include "DrawingUtil_Vulkan.h"
#include "DrawingDevice_Vulkan.h"
#include <array>

using namespace Engine;


DrawingDescriptorAllocator_Vulkan::DrawingDescriptorAllocator_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice)
	: m_pDevice(pDevice)
{
}

DrawingDescriptorAllocator_Vulkan::~DrawingDescriptorAllocator_Vulkan()
{
	Destroy();
}

void DrawingDescriptorAllocator_Vulkan::Destroy()
{
	DestroyPools();
}

void DrawingDescriptorAllocator_Vulkan::DestroyPools()
{
	for (auto& pool : m_descriptorPools)
	{
		if (pool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(m_pDevice->m_logicalDevice, pool, nullptr);
		}
	}
	m_descriptorPools.clear();
}

uint32_t DrawingDescriptorAllocator_Vulkan::CreateDescriptorPool(uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes)
{
	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = maxSets;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

	VkDescriptorPool newPool;
	if (VK_CheckResult(vkCreateDescriptorPool(m_pDevice->m_logicalDevice, &poolInfo, nullptr, &newPool), "vkCreateDescriptorPool"))
	{
		m_descriptorPools.emplace_back(newPool);
		return static_cast<uint32_t>(m_descriptorPools.size() - 1);
	}
	else
	{
		return -1;
	}
}

bool DrawingDescriptorAllocator_Vulkan::AllocateDescriptorSets(uint32_t descriptorPoolID, const std::vector<VkDescriptorSetLayout>& layouts, std::vector<VkDescriptorSet>& outputSets, bool clearPreviousSets)
{
	assert(descriptorPoolID < m_descriptorPools.size());
	assert(m_descriptorPools[descriptorPoolID] != VK_NULL_HANDLE);

	if (clearPreviousSets)
	{
		vkResetDescriptorPool(m_pDevice->m_logicalDevice, m_descriptorPools[descriptorPoolID], 0);
		outputSets.clear();
	}	

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_descriptorPools[descriptorPoolID];
	allocInfo.descriptorSetCount = static_cast<uint32_t>(layouts.size());
	allocInfo.pSetLayouts = layouts.data();

	std::vector<VkDescriptorSet> newSets(layouts.size());
	VK_CheckResult(vkAllocateDescriptorSets(m_pDevice->m_logicalDevice, &allocInfo, newSets.data()), "vkAllocateDescriptorSets");

	for (auto& newSet : newSets)
	{
		outputSets.emplace_back(newSet);
	}

	return true;
}

bool DrawingDescriptorAllocator_Vulkan::UpdateDescriptorSets(const std::vector<DescriptorUpdateInfo_Vulkan>& updateInfo)
{
	std::vector<VkWriteDescriptorSet> descriptorWrites = {};

	for (auto& info : updateInfo)
	{
		if (!info.HasContent)
		{
			continue;
		}

		VkWriteDescriptorSet descriptorWrite = {};

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = info.TargetDescriptorSet;
		descriptorWrite.dstBinding = info.TargetDescriptorBinding;
		descriptorWrite.dstArrayElement = info.TargetArrayElement;
		descriptorWrite.descriptorType = info.TargetDescriptorType;
		
		switch (info.InfoType)
		{
		case eDescriptorInfo_Buffer:
			descriptorWrite.descriptorCount = static_cast<uint32_t>(info.BufferInfos.size());
			descriptorWrite.pBufferInfo = info.BufferInfos.data();
			break;
		case eDescriptorInfo_Image:
			descriptorWrite.descriptorCount = static_cast<uint32_t>(info.ImageInfos.size());
			descriptorWrite.pImageInfo = info.ImageInfos.data();
			break;
		case eDescriptorInfo_TexelBuffer:
			descriptorWrite.descriptorCount = static_cast<uint32_t>(info.TexelBufferViews.size());
			descriptorWrite.pTexelBufferView = info.TexelBufferViews.data();
			break;
		default:
			throw std::runtime_error("Unhandled descriptor info type.");
			return false;
		}

		descriptorWrites.emplace_back(descriptorWrite);
	}

	vkUpdateDescriptorSets(m_pDevice->m_logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

	return true;
}

DrawingDescriptorSetLayout_Vulkan::DrawingDescriptorSetLayout_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice)
	: m_pDevice(pDevice), m_descriptorSetLayout(VK_NULL_HANDLE), m_numDescriptorsPerSet{ 0 }, m_descriptorSetBitMask(0)
{
}

DrawingDescriptorSetLayout_Vulkan::DrawingDescriptorSetLayout_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const std::vector<VkDescriptorSetLayoutBinding>& bindings)
	: m_pDevice(pDevice), m_numDescriptorsPerSet{ 0 }, m_descriptorSetBitMask(0)
{
	m_descriptorSetLayout = VK_NULL_HANDLE;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	VK_CheckResult(vkCreateDescriptorSetLayout(m_pDevice->m_logicalDevice, &layoutInfo, nullptr, &m_descriptorSetLayout), "vkCreateDescriptorSetLayout");
}

DrawingDescriptorSetLayout_Vulkan::~DrawingDescriptorSetLayout_Vulkan()
{
	vkDestroyDescriptorSetLayout(m_pDevice->m_logicalDevice, m_descriptorSetLayout, nullptr);
}

const VkDescriptorSetLayout DrawingDescriptorSetLayout_Vulkan::GetDescriptorSetLayout() const
{
	return m_descriptorSetLayout;
}

uint32_t DrawingDescriptorSetLayout_Vulkan::GetDescriptorSetBitMask() const
{
	return m_descriptorSetBitMask;
}

uint32_t DrawingDescriptorSetLayout_Vulkan::GetNumDescriptors(uint32_t index) const
{
	assert(index < 32);
	return m_numDescriptorsPerSet[index];
}