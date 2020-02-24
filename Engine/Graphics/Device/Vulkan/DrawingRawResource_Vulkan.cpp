#define NOMINMAX
//#define _VK_DEBUG_MSG
#include "DrawingDevice_Vulkan.h"
#include "DrawingRawResource_Vulkan.h"
#include "Macros.h"
#include <iostream>

using namespace Engine;

DrawingRawVertexFormat_Vulkan::DrawingRawVertexFormat_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, const std::vector<VkVertexInputBindingDescription>& bindingDesc, const std::vector<VkVertexInputAttributeDescription>& attribDesc)
	: m_pDevice(pDevice)
{
	std::for_each(bindingDesc.cbegin(), bindingDesc.cend(), [this](auto& elem)
	{
		m_bindingDescs.emplace_back(elem);
	});

	std::for_each(attribDesc.cbegin(), attribDesc.cend(), [this](auto& elem)
	{
		m_attribDescs.emplace_back(elem);
	});

	m_inputInfo = {};
	m_inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_inputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(m_bindingDescs.size());
	m_inputInfo.pVertexBindingDescriptions = m_bindingDescs.data();
	m_inputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_attribDescs.size());
	m_inputInfo.pVertexAttributeDescriptions = m_attribDescs.data();
}

const VkPipelineVertexInputStateCreateInfo* DrawingRawVertexFormat_Vulkan::Get() const
{
	return &m_inputInfo;
}

DrawingRawBuffer_Vulkan::DrawingRawBuffer_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, const VkBufferUsageFlags usage, const VmaMemoryUsage memoryUsage, const VkDeviceSize size, const VkDeviceSize stride)
	: m_pDevice(pDevice), m_sizeInBytes(size), m_strideInBytes(stride)
{
	m_buffer = VK_NULL_HANDLE;
	m_allocation = VK_NULL_HANDLE;

	m_pAllocator = pDevice->GetUploadAllocator();
	m_isValid = m_pAllocator->CreateBuffer(size, usage, memoryUsage, m_buffer, m_allocation);
}

DrawingRawBuffer_Vulkan::~DrawingRawBuffer_Vulkan()
{
	Free();
}

void DrawingRawBuffer_Vulkan::Free()
{
	if (m_isValid)
	{
		m_pAllocator->FreeBuffer(m_buffer, m_allocation);
		m_buffer = VK_NULL_HANDLE;
		m_allocation = VK_NULL_HANDLE;
		m_isValid = false;
	}
}

VkDeviceSize DrawingRawBuffer_Vulkan::GetSize() const
{
	return m_sizeInBytes;
}

VkDeviceSize DrawingRawBuffer_Vulkan::GetStride() const
{
	return m_strideInBytes;
}

void* DrawingRawBuffer_Vulkan::GetMappedPointer()
{
	void* data = nullptr;
	bool result = m_pAllocator->MapMemory(m_allocation, &data);
	assert(result);
	return data;
}

void DrawingRawBuffer_Vulkan::UnmapMemory()
{
	m_pAllocator->UnmapMemory(m_allocation);
}

bool DrawingRawBuffer_Vulkan::isValid() const
{
	return m_isValid && (m_allocation != VK_NULL_HANDLE) && (m_buffer != VK_NULL_HANDLE);
}

DrawingRawVertexBuffer_Vulkan::DrawingRawVertexBuffer_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, const void* pData, const VkDeviceSize size, const VkDeviceSize stride)
	: m_pDevice(pDevice)
{
	m_pAllocator = pDevice->GetUploadAllocator();

#ifndef DIRECT_MAPPED_BUFFER_VK

#if defined(POOLED_STAGING_BUFFER_VK)
	auto pStagingBufferPool = m_pDevice->GetStagingBufferPool();
	uint32_t stagingBufferID = pStagingBufferPool->RequestStagingBufferID(size);
	auto pStagingBuffer = pStagingBufferPool->RequestStagingBuffer(stagingBufferID);

	void* data;
	m_pAllocator->MapMemory(pStagingBuffer->m_allocation, &data);
	memcpy(data, pData, size);
	m_pAllocator->UnmapMemory(pStagingBuffer->m_allocation);

	VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;
	m_pBufferImpl = std::make_shared<DrawingRawBuffer_Vulkan>(pDevice, usage, memoryUsage, size, stride);

	m_pAllocator->CopyBuffer_Immediate(pStagingBuffer->m_buffer, m_pBufferImpl->m_buffer, size);

	pStagingBufferPool->ReturnStagingBuffer(stagingBufferID);
#else
	VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_CPU_ONLY; // See https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/usage_patterns.html
	DrawingRawBuffer_Vulkan stagingBuffer(pDevice, usage, memoryUsage, size);

	void* data;
	m_pAllocator->MapMemory(stagingBuffer.m_allocation, &data);
	memcpy(data, pData, size);
	m_pAllocator->UnmapMemory(stagingBuffer.m_allocation);

	usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;
	m_pBufferImpl = std::make_shared<DrawingRawBuffer_Vulkan>(pDevice, usage, memoryUsage, size, stride);

	m_pAllocator->CopyBuffer_Immediate(stagingBuffer.m_buffer, m_pBufferImpl->m_buffer, size);

	stagingBuffer.Free();
#endif

#else
	VkBufferUsageFlags usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;
	m_pBufferImpl = std::make_shared<DrawingRawBuffer_Vulkan>(pDevice, usage, memoryUsage, size, stride);

	if (size > 0 && pData != nullptr)
	{
		void* data;
		m_pAllocator->MapMemory(m_pBufferImpl->m_allocation, &data);
		memcpy(data, pData, size);
		m_pAllocator->UnmapMemory(m_pBufferImpl->m_allocation);
	}
#endif
}

DrawingRawVertexBuffer_Vulkan::~DrawingRawVertexBuffer_Vulkan()
{
	Free();
}

VkBuffer DrawingRawVertexBuffer_Vulkan::GetBuffer() const
{
	return m_pBufferImpl->m_buffer;
}

VkDeviceSize DrawingRawVertexBuffer_Vulkan::GetSize() const
{
	return m_pBufferImpl->GetSize();
}

VkDeviceSize DrawingRawVertexBuffer_Vulkan::GetStride() const
{
	return m_pBufferImpl->GetStride();
}

void* DrawingRawVertexBuffer_Vulkan::MapResource()
{
	return m_pBufferImpl->GetMappedPointer();
}

void DrawingRawVertexBuffer_Vulkan::UnmapResource()
{
	m_pBufferImpl->UnmapMemory();
}

void DrawingRawVertexBuffer_Vulkan::Free()
{
	m_pBufferImpl->Free();
}

DrawingRawIndexBuffer_Vulkan::DrawingRawIndexBuffer_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, const void* pData, VkDeviceSize size, VkDeviceSize stride)
{
	auto m_pAllocator = pDevice->GetUploadAllocator();

#ifndef DIRECT_MAPPED_BUFFER_VK

#if defined(POOLED_STAGING_BUFFER_VK)
	auto pStagingBufferPool = m_pDevice->GetStagingBufferPool();
	uint32_t stagingBufferID = pStagingBufferPool->RequestStagingBufferID(size);
	auto pStagingBuffer = pStagingBufferPool->RequestStagingBuffer(stagingBufferID);

	void* data;
	m_pAllocator->MapMemory(pStagingBuffer->m_allocation, &data);
	memcpy(data, pData, size);
	m_pAllocator->UnmapMemory(pStagingBuffer->m_allocation);

	VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;
	m_pBufferImpl = std::make_shared<DrawingRawBuffer_Vulkan>(pDevice, usage, memoryUsage, size, stride);

	m_pAllocator->CopyBuffer_Immediate(pStagingBuffer->m_buffer, m_pBufferImpl->m_buffer, size);

	pStagingBufferPool->ReturnStagingBuffer(stagingBufferID);
#else
	VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_CPU_ONLY;
	DrawingRawBuffer_Vulkan stagingBuffer(pDevice, usage, memoryUsage, size);

	void* data;
	m_pAllocator->MapMemory(stagingBuffer.m_allocation, &data);
	memcpy(data, pData, size);
	m_pAllocator->UnmapMemory(stagingBuffer.m_allocation);

	usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;
	m_pBufferImpl = std::make_shared<DrawingRawBuffer_Vulkan>(pDevice, usage, memoryUsage, size, stride);

	m_pAllocator->CopyBuffer_Immediate(stagingBuffer.m_buffer, m_pBufferImpl->m_buffer, size);

	stagingBuffer.Free();
#endif

#else
	VkBufferUsageFlags usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;
	m_pBufferImpl = std::make_shared<DrawingRawBuffer_Vulkan>(pDevice, usage, memoryUsage, size, stride);

	if (size > 0 && pData != nullptr)
	{
		void* data;
		m_pAllocator->MapMemory(m_pBufferImpl->m_allocation, &data);
		memcpy(data, pData, size);
		m_pAllocator->UnmapMemory(m_pBufferImpl->m_allocation);
	}
#endif

	m_format = (stride == 2) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;
}

DrawingRawIndexBuffer_Vulkan::~DrawingRawIndexBuffer_Vulkan()
{
	Free();
}

VkBuffer DrawingRawIndexBuffer_Vulkan::GetBuffer() const
{
	return m_pBufferImpl->m_buffer;
}

VkDeviceSize DrawingRawIndexBuffer_Vulkan::GetSize() const
{
	return m_pBufferImpl->GetSize();
}

VkDeviceSize DrawingRawIndexBuffer_Vulkan::GetStride() const
{
	return m_pBufferImpl->GetStride();
}

VkIndexType DrawingRawIndexBuffer_Vulkan::GetFormat() const
{
	return m_format;
}

void* DrawingRawIndexBuffer_Vulkan::MapResource()
{
	return m_pBufferImpl->GetMappedPointer();
}

void DrawingRawIndexBuffer_Vulkan::UnmapResource()
{
	m_pBufferImpl->UnmapMemory();
}

void DrawingRawIndexBuffer_Vulkan::Free()
{
	m_pBufferImpl->Free();
}

DrawingRawShader_Vulkan::DrawingRawShader_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, VkShaderModule shaderModule, const VkShaderStageFlagBits stage, const char* entry)
	: DrawingRawShader_Common(), m_pDevice(pDevice), m_shaderModule(shaderModule), m_entryName(entry), m_stage(stage)
{}

DrawingRawShader_Vulkan::~DrawingRawShader_Vulkan()
{
	if (m_shaderModule != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(m_pDevice->m_logicalDevice, m_shaderModule, nullptr);
		m_shaderModule = VK_NULL_HANDLE;

		m_rawCode.clear();
	}
}

VkShaderModule DrawingRawShader_Vulkan::GetModule() const
{
	return m_shaderModule;
}

const char* DrawingRawShader_Vulkan::EntryName() const
{
	return m_entryName;
}

VkShaderStageFlagBits DrawingRawShader_Vulkan::GetStage() const
{
	return m_stage;
}

void DrawingRawShader_Vulkan::ProcessVariables(std::shared_ptr<std::string> pName, const spirv_cross::Compiler& comp, const spirv_cross::Resource& resource)
{
	auto resourceType = comp.get_type(resource.base_type_id);
	uint32_t memberCount = static_cast<uint32_t>(resourceType.member_types.size());
	size_t resourceSize = comp.get_declared_struct_size(resourceType);

	for (uint32_t i = 0; i < memberCount; ++i)
	{
		VariableDesc imp;
		imp.mpName = strPtr(comp.get_member_name(resource.base_type_id, i));
		imp.mpCBName = pName;
		imp.mOffset = comp.type_struct_member_offset(resourceType, i);
		imp.mSizeInBytes = static_cast<uint32_t>(comp.get_declared_struct_member_size(resourceType, i));
		imp.mCBSizeInBytes = static_cast<uint32_t>(resourceSize);

		auto varType = comp.get_type(resourceType.member_types[i]);
		uint32_t varSize = i < (memberCount - 1)
			? (comp.type_struct_member_offset(resourceType, i + 1) - imp.mOffset)
			: (static_cast<uint32_t>(resourceSize) - imp.mOffset);

		imp.mParamType = GetParamType_Vulkan(varType, varSize);

		mVariableTable.emplace(imp.mpName, imp);
	}
}

void DrawingRawShader_Vulkan::BuildResourceBindingTable(DrawingRawShader::DrawingRawShaderType type)
{
	size_t wordCount = m_rawCode.size() * sizeof(char) / sizeof(uint32_t);
	assert(wordCount > 0);
	std::vector<uint32_t> rawCode(wordCount);
	memcpy(rawCode.data(), m_rawCode.data(), m_rawCode.size());

	spirv_cross::Compiler comp(move(rawCode));

	// Query only active variables
	auto active = comp.get_active_interface_variables();
	spirv_cross::ShaderResources res = comp.get_shader_resources(active);
	comp.set_enabled_interface_variables(move(active));

	// Alert: only [uniform buffer], [push constant] and [separate image] have been tested to be correct

#if defined(_VK_DEBUG_MSG)
	std::cout << "Shader stage: " << m_stage << std::endl;
	std::cout << "Uniform buffer count: " << res.uniform_buffers.size() << std::endl;
#endif
	for (const spirv_cross::Resource& resource : res.uniform_buffers)
	{
		ShaderResourceDesc imp;
		imp.mStartSlot = comp.get_decoration(resource.id, spv::DecorationBinding);
		assert(imp.mStartSlot < m_pDevice->GetDeviceLimits().maxDescriptorSetUniformBuffers);
		imp.mCount = 1;
		imp.mpName = strPtr(comp.get_name(resource.id));
		imp.mIsPushConstant = false;

		mCBTable.emplace(imp.mpName, imp);
		ProcessVariables(imp.mpName, comp, resource);
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Push constant buffer count: " << res.push_constant_buffers.size() << std::endl;
#endif
	size_t pushConstSize = 0;
	for (const spirv_cross::Resource& resource : res.push_constant_buffers)
	{
		pushConstSize += comp.get_declared_struct_size(comp.get_type(resource.base_type_id));
		assert(pushConstSize <= static_cast<size_t>(m_pDevice->GetDeviceLimits().maxPushConstantsSize));
		ShaderResourceDesc imp;
		imp.mStartSlot = comp.get_decoration(resource.id, spv::DecorationBinding);
		assert(imp.mStartSlot < m_pDevice->GetDeviceLimits().maxDescriptorSetUniformBuffers);
		imp.mCount = 1;
		imp.mpName = strPtr(comp.get_name(resource.id));
		imp.mIsPushConstant = true;

		mCBTable.emplace(imp.mpName, imp);	
		ProcessVariables(imp.mpName, comp, resource);
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Storage buffer count: " << res.storage_buffers.size() << std::endl;
#endif
	for (const spirv_cross::Resource& resource : res.storage_buffers)
	{
		ShaderResourceDesc imp;
		imp.mStartSlot = comp.get_decoration(resource.id, spv::DecorationBinding);
		assert(imp.mStartSlot < m_pDevice->GetDeviceLimits().maxDescriptorSetStorageBuffers);
		imp.mCount = 1;
		imp.mpName = strPtr(comp.get_name(resource.id));

		mUVATable.emplace(imp.mpName, imp); // Alert: this may be incorrect
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Subpass input count: " << res.subpass_inputs.size() << std::endl;
#endif
	for (const spirv_cross::Resource& resource : res.subpass_inputs)
	{
		throw std::runtime_error("Vulkan shader has used subpass input(s) which has not been implemented.");
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Storage image count: " << res.storage_images.size() << std::endl;
#endif
	for (const spirv_cross::Resource& resource : res.storage_images)
	{
		ShaderResourceDesc imp;
		imp.mStartSlot = comp.get_decoration(resource.id, spv::DecorationBinding);
		assert(imp.mStartSlot < m_pDevice->GetDeviceLimits().maxDescriptorSetStorageImages);
		imp.mCount = 1;
		imp.mpName = strPtr(comp.get_name(resource.id));

		mTexBufferTable.emplace(imp.mpName, imp); // Alert: this may be incorrect
	}
	
#if defined(_VK_DEBUG_MSG)
	std::cout << "Sampled image count: " << res.sampled_images.size() << std::endl;
#endif
	for (const spirv_cross::Resource& resource : res.sampled_images)
	{
		ShaderResourceDesc imp;
		imp.mStartSlot = comp.get_decoration(resource.id, spv::DecorationBinding);
		assert(imp.mStartSlot < m_pDevice->GetDeviceLimits().maxDescriptorSetSampledImages);
		imp.mCount = 1;
		imp.mpName = strPtr(comp.get_name(resource.id));

		mSamplerTable.emplace(imp.mpName, imp); // Alert: this may be incorrect
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Separate image count: " << res.separate_images.size() << std::endl;
#endif
	for (const spirv_cross::Resource& resource : res.separate_images)
	{
		ShaderResourceDesc imp;
		imp.mStartSlot = comp.get_decoration(resource.id, spv::DecorationBinding);
		assert(imp.mStartSlot < m_pDevice->GetDeviceLimits().maxDescriptorSetSampledImages); // Alert: this may be incorrect
		imp.mCount = 1;
		imp.mpName = strPtr(comp.get_name(resource.id));

		mTextureTable.emplace(imp.mpName, imp);
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Separate sampler count: " << res.separate_samplers.size() << std::endl;
#endif
	for (const spirv_cross::Resource& resource : res.separate_samplers)
	{
		ShaderResourceDesc imp;
		imp.mStartSlot = comp.get_decoration(resource.id, spv::DecorationBinding);
		assert(imp.mStartSlot < m_pDevice->GetDeviceLimits().maxDescriptorSetSamplers); // Alert: this may be incorrect
		imp.mCount = 1;
		imp.mpName = strPtr(comp.get_name(resource.id));

		mSamplerTable.emplace(imp.mpName, imp); // Alert: this may be incorrect
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Acceleration structure count: " << res.acceleration_structures.size() << std::endl;
#endif
	for (const spirv_cross::Resource& resource : res.acceleration_structures)
	{
		throw std::runtime_error("Vulkan shader has used acceleration structure(s) which has not been implemented.");
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan RawShader-BuildResourceBindingTable [Partial]" << std::endl;
#endif
}

DrawingRawVertexShader_Vulkan::DrawingRawVertexShader_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<std::string> pShaderName, VkShaderModule shaderModule, std::vector<char>& rawCode, const char* entry)
	: DrawingRawVertexShader(pShaderName), m_pShaderImpl(std::make_shared<DrawingRawShader_Vulkan>(pDevice, shaderModule, VK_SHADER_STAGE_VERTEX_BIT, entry)), m_stage(VK_SHADER_STAGE_VERTEX_BIT)
{
	assert(m_pShaderImpl != nullptr);
	m_pShaderImpl->m_rawCode = rawCode;
	m_pShaderImpl->BuildResourceBindingTable(GetShaderType());
}

VkShaderModule DrawingRawVertexShader_Vulkan::Get() const
{
	return m_pShaderImpl->GetModule();
}

std::shared_ptr<DrawingRawShader_Vulkan> DrawingRawVertexShader_Vulkan::GetShaderImpl() const
{
	return m_pShaderImpl;
}

VkShaderStageFlagBits DrawingRawVertexShader_Vulkan::GetStage() const
{
	return m_stage;
}

DrawingRawFragmentShader_Vulkan::DrawingRawFragmentShader_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<std::string> pShaderName, VkShaderModule shaderModule, std::vector<char>& rawCode, const char* entry)
	: DrawingRawPixelShader(pShaderName), m_pShaderImpl(std::make_shared<DrawingRawShader_Vulkan>(pDevice, shaderModule, VK_SHADER_STAGE_FRAGMENT_BIT, entry)), m_stage(VK_SHADER_STAGE_FRAGMENT_BIT)
{
	assert(m_pShaderImpl != nullptr);
	m_pShaderImpl->m_rawCode = rawCode;
	m_pShaderImpl->BuildResourceBindingTable(GetShaderType());
}

VkShaderModule DrawingRawFragmentShader_Vulkan::Get() const
{
	return m_pShaderImpl->GetModule();
}

std::shared_ptr<DrawingRawShader_Vulkan> DrawingRawFragmentShader_Vulkan::GetShaderImpl() const
{
	return m_pShaderImpl;
}

VkShaderStageFlagBits DrawingRawFragmentShader_Vulkan::GetStage() const
{
	return m_stage;
}

DrawingRawImage_Vulkan::DrawingRawImage_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const RawImageCreateInfo_Vulkan& createInfo)
	: m_pDevice(pDevice), m_layout(VK_IMAGE_LAYOUT_UNDEFINED), m_image(VK_NULL_HANDLE), m_allocation(VK_NULL_HANDLE), m_imageView(VK_NULL_HANDLE),
	m_extent({0, 0, 0}), m_format(VK_FORMAT_UNDEFINED), m_mipLevels(0)
{
	m_pAllocator = m_pDevice->GetUploadAllocator();
	m_pAllocator->CreateImage(createInfo, *this);

	m_imageView = m_pDevice->CreateImageView(m_image, createInfo.viewType, createInfo.aspect, createInfo.format, createInfo.mipLevels);

	m_isValid = true;
}

DrawingRawImage_Vulkan::DrawingRawImage_Vulkan(const VkImage targetImage, const VkImageView targetView, const VkExtent2D targetExtent, const VkFormat targetFormat)
	: m_image(targetImage), m_imageView(targetView), m_extent({ targetExtent.width, targetExtent.height, 1 }), m_format(targetFormat), m_layout(VK_IMAGE_LAYOUT_UNDEFINED), m_mipLevels(1), m_isValid(true), m_pAllocator(nullptr), m_allocation(VK_NULL_HANDLE)
{
}

DrawingRawImage_Vulkan::~DrawingRawImage_Vulkan()
{
	Free();
}

bool DrawingRawImage_Vulkan::isValid() const
{
	return m_isValid && (m_image != VK_NULL_HANDLE) && (m_allocation != VK_NULL_HANDLE);
}

void DrawingRawImage_Vulkan::Free()
{
	if (m_isValid)
	{
		if (m_pAllocator != nullptr)
		{
			m_pAllocator->FreeImage(m_image, m_allocation);
			m_image = VK_NULL_HANDLE;
			m_allocation = VK_NULL_HANDLE;
		}
		else
		{
			vkDestroyImage(m_pDevice->m_logicalDevice, m_image, nullptr);
			m_image = VK_NULL_HANDLE;
		}
		if (m_imageView != VK_NULL_HANDLE)
		{
			vkDestroyImageView(m_pDevice->m_logicalDevice, m_imageView, nullptr);
			m_imageView = VK_NULL_HANDLE;
		}
		m_isValid = false;
	}
}

DrawingRawTarget_Vulkan::DrawingRawTarget_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<DrawingRawImage_Vulkan> rawImage)
	: m_pDevice(pDevice), m_pTargetImpl(rawImage)
{
}

std::shared_ptr<DrawingRawImage_Vulkan> DrawingRawTarget_Vulkan::GetRenderTarget() const
{
	return m_pTargetImpl;
}

VkImageView DrawingRawTarget_Vulkan::GetTargetView() const
{
	return m_pTargetImpl->m_imageView;
}

VkFormat DrawingRawTarget_Vulkan::GetTargetFormat() const
{
	return m_pTargetImpl->m_format;
}

VkExtent2D DrawingRawTarget_Vulkan::GetExtent() const
{
	return { m_pTargetImpl->m_extent.width, m_pTargetImpl->m_extent.height };
}

DrawingRawTarget::ETargetType DrawingRawTarget_Vulkan::GetTargetType() const
{
	return eTarget_SwapChain;
}

DrawingRawDepthTarget_Vulkan::DrawingRawDepthTarget_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, RawImageCreateInfo_Vulkan& createInfo)
	: DrawingRawTarget_Vulkan(pDevice, std::make_shared<DrawingRawImage_Vulkan>(pDevice, createInfo))
{
}

DrawingRawTarget::ETargetType DrawingRawDepthTarget_Vulkan::GetTargetType() const
{
	return eTarget_Depth;
}

DrawingRawFragmentTarget_Vulkan::DrawingRawFragmentTarget_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice)
	: DrawingRawTarget_Vulkan(pDevice, nullptr), m_bufferIndex(0)
{
}

DrawingRawFragmentTarget_Vulkan::DrawingRawFragmentTarget_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<DrawingRawImage_Vulkan> pImage)
	: DrawingRawTarget_Vulkan(pDevice, pImage), m_bufferIndex(0) 
{
}

uint32_t DrawingRawFragmentTarget_Vulkan::GetCurrentIndex() const
{
	return m_bufferIndex;
}

DrawingRawSwapchain_Vulkan::DrawingRawSwapchain_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, SwapchainCreateInfo_Vulkan& createInfo)
	: DrawingRawFragmentTarget_Vulkan(pDevice)
{
	uint32_t imageCount = createInfo.swapchainSupport.capabilities.minImageCount - 1 + createInfo.maxFramesInFlight;
	if (createInfo.swapchainSupport.capabilities.maxImageCount > 0 && imageCount > createInfo.swapchainSupport.capabilities.maxImageCount)
	{
		imageCount = createInfo.swapchainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = createInfo.surface;
	swapchainCreateInfo.minImageCount = imageCount;
	swapchainCreateInfo.imageFormat = createInfo.surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = createInfo.surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = createInfo.swapExtent;
	swapchainCreateInfo.imageArrayLayers = 1; // Alert: this could be different
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

	uint32_t queueFamilyIndices[] = { createInfo.queueFamilyIndices.graphicsFamily.value(), createInfo.queueFamilyIndices.presentFamily.value() };

	if (createInfo.queueFamilyIndices.graphicsFamily != createInfo.queueFamilyIndices.presentFamily)
	{
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount = 2;
		swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	vkGetDeviceQueue(pDevice->m_logicalDevice, createInfo.queueFamilyIndices.presentFamily.value(), 0, &m_presentQueue);

	swapchainCreateInfo.preTransform = createInfo.swapchainSupport.capabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkCompositeAlphaFlagBitsKHR.html
	swapchainCreateInfo.presentMode = createInfo.presentMode;
	swapchainCreateInfo.clipped = VK_TRUE; // https://khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkSwapchainCreateInfoKHR.html

	VkResult result = vkCreateSwapchainKHR(pDevice->m_logicalDevice, &swapchainCreateInfo, nullptr, &m_swapchain);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create swapchain.");
	}

	vkGetSwapchainImagesKHR(pDevice->m_logicalDevice, m_swapchain, &imageCount, nullptr);

	std::vector<VkImage> swapchainImages(imageCount, VK_NULL_HANDLE);
	vkGetSwapchainImagesKHR(pDevice->m_logicalDevice, m_swapchain, &imageCount, swapchainImages.data());

	for (uint32_t i = 0; i < imageCount; ++i)
	{
		VkImageView swapchainView = pDevice->CreateImageView(swapchainImages[i], VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, createInfo.surfaceFormat.format, 1);
		m_pTargetArray.push_back(std::make_shared<DrawingRawTarget_Vulkan>(pDevice, std::make_shared<DrawingRawImage_Vulkan>(swapchainImages[i], swapchainView, createInfo.swapExtent, createInfo.surfaceFormat.format)));
	}

	m_isValid = true;

	// This is a fix to the first read of swapchain format and extent data
	m_pTargetImpl = m_pTargetArray[0]->GetRenderTarget();
}

void DrawingRawSwapchain_Vulkan::Free()
{
	if (m_isValid)
	{
		for (auto target : m_pTargetArray)
		{
			vkDestroyImageView(m_pDevice->m_logicalDevice, target->GetTargetView(), nullptr);
		}

		for (auto framebuffer : m_swapchainFramebuffers)
		{
			vkDestroyFramebuffer(m_pDevice->m_logicalDevice, framebuffer, nullptr);
		}

		vkDestroySwapchainKHR(m_pDevice->m_logicalDevice, m_swapchain, nullptr);

		m_isValid = false;
	}
}

DrawingRawSwapchain_Vulkan::~DrawingRawSwapchain_Vulkan()
{
	Free();
}

VkResult DrawingRawSwapchain_Vulkan::UpdateBackBuffer(VkSemaphore semaphore, uint64_t timeout)
{
	VkResult result = vkAcquireNextImageKHR(m_pDevice->m_logicalDevice, m_swapchain, timeout, semaphore, VK_NULL_HANDLE, &m_bufferIndex);
	m_pTargetImpl = m_pTargetArray[m_bufferIndex]->GetRenderTarget();

	return result;
}

bool DrawingRawSwapchain_Vulkan::CreateFramebuffers(VkImageView depthImageView, VkRenderPass renderPass)
{
	m_swapchainFramebuffers.resize(m_pTargetArray.size());

	for (size_t i = 0; i < m_pTargetArray.size(); ++i)
	{
		std::array<VkImageView, 2> attachments =
		{
			m_pTargetArray[i]->GetTargetView(),
			depthImageView
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = GetExtent().width;
		framebufferInfo.height = GetExtent().height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(m_pDevice->m_logicalDevice, &framebufferInfo, nullptr, &m_swapchainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create framebuffer.");
			return false;
		}
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreateFramebuffers [Implemented]" << std::endl;
#endif

	return true;
}

DrawingRawTarget::ETargetType DrawingRawSwapchain_Vulkan::GetTargetType() const
{
	return eTarget_SwapChain;
}

size_t DrawingRawSwapchain_Vulkan::GetTargetCount() const
{
	return m_pTargetArray.size();
}

VkFramebuffer DrawingRawSwapchain_Vulkan::GetTargetFramebuffer() const
{
	assert(m_bufferIndex < m_swapchainFramebuffers.size());
	return m_swapchainFramebuffers[m_bufferIndex];
}

VkImageView DrawingRawSwapchain_Vulkan::GetTargetViewAt(uint32_t index) const
{
	assert(index < m_pTargetArray.size());
	return m_pTargetArray[index]->GetTargetView();
}

VkResult DrawingRawSwapchain_Vulkan::Present(const std::vector<std::shared_ptr<SignalSempahore_Vulkan>>& waitSemaphores, uint32_t syncInterval)
{
	if (m_swapchain != VK_NULL_HANDLE)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		std::vector<VkSemaphore> semaphoresToWait;
		for (auto& semaphore : waitSemaphores)
		{
			semaphoresToWait.emplace_back(semaphore->semaphore);
		}

		presentInfo.waitSemaphoreCount = static_cast<uint32_t>(semaphoresToWait.size());
		presentInfo.pWaitSemaphores = semaphoresToWait.data();

		VkSwapchainKHR swapchains[] = { m_swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;

		uint32_t indices[] = { m_bufferIndex };
		presentInfo.pImageIndices = indices;

		return vkQueuePresentKHR(m_presentQueue, &presentInfo); // This is not a good practice, since we could have combined present operation for multiple swapchain
	}
	return VK_NOT_READY;
}

DrawingRawEffect_Vulkan::DrawingRawEffect_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<std::string> pEffectName)
	: DrawingRawEffect(pEffectName), m_pDevice(pDevice)
{
	m_pDescriptorSetLayout = std::make_shared<DrawingDescriptorSetLayout_Vulkan>(pDevice);
}

std::shared_ptr<DrawingDescriptorSetLayout_Vulkan> DrawingRawEffect_Vulkan::GetDescriptorSetLayoutObject() const
{
	return m_pDescriptorSetLayout;
}

bool DrawingRawSwapchain_Vulkan::InitializeTargetLayout()
{
	for (size_t i = 0; i < m_pTargetArray.size(); ++i)
	{
		auto pCommandManager = m_pDevice->GetCommandManager(eCommandList_Direct);
		pCommandManager->TransitionImageLayout_Immediate(m_pTargetArray[i]->GetRenderTarget(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	}

	return true;
}

DrawingRawShaderEffect_Vulkan::DrawingRawShaderEffect_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<std::string> pEffectName, std::shared_ptr<DrawingRawVertexShader_Vulkan> pVertexShader, std::shared_ptr<DrawingRawFragmentShader_Vulkan> pFragmentShader, uint32_t maxSetsCount)
	: DrawingRawEffect_Vulkan(pDevice, pEffectName), m_descriptorPoolID(-1), m_descriptorDirty(true)
{
	assert(pVertexShader != nullptr && pFragmentShader != nullptr);

	auto pVSShaderImpl = pVertexShader->GetShaderImpl();
	auto pFSShaderImpl = pFragmentShader->GetShaderImpl();

	if (pVSShaderImpl != nullptr)
	{
		m_shaderBlocks[DrawingRawShader::RawShader_VS] = pVertexShader;
		LoadShaderInfo(pVSShaderImpl, DrawingRawShader::RawShader_VS, maxSetsCount);
	}
	if (pFSShaderImpl != nullptr)
	{
		m_shaderBlocks[DrawingRawShader::RawShader_PS] = pFragmentShader;
		LoadShaderInfo(pFSShaderImpl, DrawingRawShader::RawShader_PS, maxSetsCount);
	}

	CreateDescriptorSetLayout();
	CreateDescriptorPool(maxSetsCount);
	AllocateDescriptorSets(maxSetsCount / 2); // We allocate 1/2 of the pool capacity
}

void DrawingRawShaderEffect_Vulkan::Apply()
{
	UpdateParameterValues();
	UpdateUniformBuffers();
	UpdateDescriptorSets();

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan ShaderEffect::Apply [Implemented]" << std::endl;
#endif
}

void DrawingRawShaderEffect_Vulkan::Terminate()
{
}

std::shared_ptr<DrawingRawShader> DrawingRawShaderEffect_Vulkan::GetRawShader(DrawingRawShader::DrawingRawShaderType type) const
{
	return m_shaderBlocks[type];
}

const VkPushConstantRange* DrawingRawShaderEffect_Vulkan::GetPushConstantRanges() const
{
	return m_pushConstantRanges.data();
}

size_t DrawingRawShaderEffect_Vulkan::PushConstantSize() const
{
	return m_pushConstantRanges.size();
}

void DrawingRawShaderEffect_Vulkan::DescriptorDirty()
{
	m_descriptorDirty = true;
}

void DrawingRawShaderEffect_Vulkan::CollectVariables(DrawingDevice::ConstBufferPropTable& cbPropTable, const DrawingRawShader_Vulkan* pShader)
{
	const auto& varTable = pShader->GetVariableTable();
	for (auto iter = varTable.begin(); iter != varTable.end(); ++iter)
	{
		auto& varDesc = iter->second;
		DrawingDevice::VarProp var_prop;
		var_prop.mpName = varDesc.mpName;
		var_prop.mType = varDesc.mParamType;
		var_prop.mSizeInBytes = varDesc.mSizeInBytes;
		var_prop.mOffset = varDesc.mOffset;
		auto cbPropIt = cbPropTable.find(varDesc.mpCBName);
		if (cbPropIt == cbPropTable.end())
		{
			DrawingDevice::ConstBufferProp local_cb_prop;
			local_cb_prop.mpName = varDesc.mpCBName;
			local_cb_prop.mSizeInBytes = varDesc.mCBSizeInBytes;
			local_cb_prop.mVarProps.emplace_back(var_prop);
			cbPropTable.emplace(varDesc.mpCBName, local_cb_prop);
		}
		else
		{
			auto cbProp = cbPropIt->second;
			if ((cbProp.mpName != varDesc.mpCBName) ||
				(cbProp.mSizeInBytes != varDesc.mCBSizeInBytes))
			{
				assert(false);
				continue;
			}
			cbProp.mVarProps.emplace_back(var_prop);
		}
	}
}

void DrawingRawShaderEffect_Vulkan::SortVariables(DrawingDevice::ConstBufferPropTable& cbPropTable)
{
	for (auto iter = cbPropTable.begin(); iter != cbPropTable.end(); ++iter)
	{
		DrawingDevice::ConstBufferProp& cbProp = iter->second;
		std::qsort(cbProp.mVarProps.data(), cbProp.mVarProps.size(), sizeof(DrawingDevice::VarProp), [](const void* a, const void* b)
			{
				const DrawingDevice::VarProp& var1 = *(static_cast<const DrawingDevice::VarProp*>(a));
				const DrawingDevice::VarProp& var2 = *(static_cast<const DrawingDevice::VarProp*>(b));
				if (var1.mOffset < var2.mOffset)
					return -1;
				else if (var1.mOffset > var2.mOffset)
					return 1;
				return 0;
			});
	}
}

void DrawingRawShaderEffect_Vulkan::BuildUBPropTable(DrawingDevice::ConstBufferPropTable& cbPropTable, const DrawingRawShader_Vulkan* pShader)
{
	CollectVariables(cbPropTable, pShader);
	SortVariables(cbPropTable);
}

void DrawingRawShaderEffect_Vulkan::BindUniformBuffer(DrawingDevice::ConstBufferPropTable& cbPropTable, const DrawingRawShader_Vulkan* pShader, const DrawingRawShader::DrawingRawShaderType shaderType)
{
	for (auto& lItem : pShader->GetConstanceBufferTable())
	{
		auto& desc = lItem.second;

		auto & cbPropIt = cbPropTable.find(desc.mpName);
		if (cbPropIt == cbPropTable.end())
		{
			continue;
		}

		auto & cbProp = cbPropIt->second;
		auto pDevCBProp = m_pDevice->FindConstantBuffer(cbProp);

		if (nullptr == pDevCBProp)
		{
			cbProp.mpCB = std::make_shared<DrawingRawUniformBuffer_Vulkan>(m_pDevice, cbProp.mSizeInBytes, desc.mStartSlot);

			if (desc.mIsPushConstant)
			{
				std::dynamic_pointer_cast<DrawingRawUniformBuffer_Vulkan>(cbProp.mpCB)->MarkAsPushConstant(pShader->GetStage());
			}
			else
			{
				std::dynamic_pointer_cast<DrawingRawUniformBuffer_Vulkan>(cbProp.mpCB)->MarkAsUniformBuffer();
				m_pUniformBuffers.push_back(std::dynamic_pointer_cast<DrawingRawUniformBuffer_Vulkan>(cbProp.mpCB)); // Record the uniform buffer info for creating descriptors, this may not be a good solution
			}

			m_pDevice->AddConstantBuffer(cbProp);
		}
		else
			cbProp.mpCB = pDevCBProp->mpCB;

		auto& ubIt = m_uniformBufferTable.find(desc.mpName);
		if (ubIt == m_uniformBufferTable.end())
		{
			SUniformBuffer local_cb_desc;
			local_cb_desc.mpName = cbProp.mpName;
			local_cb_desc.mSizeInBytes = cbProp.mSizeInBytes;
			local_cb_desc.mpUB = cbProp.mpCB;
			local_cb_desc.mStartSlot[shaderType] = desc.mStartSlot;

			m_uniformBufferTable.emplace(desc.mpName, local_cb_desc);
		}
		else
			(ubIt->second).mStartSlot[shaderType] = desc.mStartSlot;
	}
}

void DrawingRawShaderEffect_Vulkan::GenerateParameters(const DrawingRawShader_Vulkan* pShader, const DrawingRawShader::DrawingRawShaderType shaderType)
{
	for (auto& lItem : pShader->GetVariableTable())
	{
		auto& desc = lItem.second;
		auto& varIt = m_varTable.find(desc.mpName);
		auto& cbIt = m_uniformBufferTable.find(desc.mpCBName);

		if (varIt == m_varTable.end())
		{
			SParamVar local_var_desc;

			local_var_desc.mpParam = std::make_shared<DrawingParameter>(desc.mpName, desc.mParamType);
			local_var_desc.mSizeInBytes = desc.mSizeInBytes;
			local_var_desc.mOffset[shaderType] = desc.mOffset;
			if (cbIt != m_uniformBufferTable.end())
				local_var_desc.mpCB[shaderType] = (cbIt->second).mpUB;
			else
				local_var_desc.mpCB[shaderType] = nullptr;

			m_varTable.emplace(desc.mpName, local_var_desc);
			m_pParamSet->Add(local_var_desc.mpParam);
		}

		else
		{
			auto& var = varIt->second;
			if (var.mpParam->GetType() != desc.mParamType)
			{
				assert(false);
				continue;
			}

			var.mOffset[shaderType] = desc.mOffset;
			var.mpCB[shaderType] = cbIt != m_uniformBufferTable.end() ? (cbIt->second).mpUB : nullptr;
		}
	}
}

void DrawingRawShaderEffect_Vulkan::BuildTextureTable(const DrawingRawShader_Vulkan* pShader, const DrawingRawShader::DrawingRawShaderType shaderType)
{
	for (auto& item : pShader->GetTextureTable())
	{
		auto& desc = item.second;
		auto paramType = COMPOSE_TYPE(eObject_Texture, eDataSet_Object, eBasic_FP32, desc.mCount <= 1 ? 0 : desc.mCount, 0, 0);
		CheckAndAddResource(desc, paramType, shaderType, m_texTable);
	}
}

void DrawingRawShaderEffect_Vulkan::CheckAndAddResource(const DrawingRawShader_Common::ShaderResourceDesc& desc, uint32_t paramType, const DrawingRawShader::DrawingRawShaderType shaderType, std::unordered_map<std::shared_ptr<std::string>, SParamRes>& resTable) const
{
	auto paramIndex = m_pParamSet->IndexOfName(desc.mpName);

	if (paramIndex != DrawingParameterSet::npos)
	{
		assert((*m_pParamSet)[paramIndex] != nullptr && (*m_pParamSet)[paramIndex]->GetType() == paramType);

		auto iter = resTable.find(desc.mpName);
		if (iter != resTable.end())
			(iter->second).mStartSlot[shaderType] = desc.mStartSlot;
	}
	else
	{
		auto pParam = std::make_shared<DrawingParameter>(desc.mpName, paramType);
		m_pParamSet->Add(pParam);

		SParamRes paramRes;
		paramRes.mpParam = pParam;
		paramRes.mCount = desc.mCount;
		paramRes.mStartSlot[shaderType] = desc.mStartSlot;

		resTable.emplace(desc.mpName, paramRes);
	}
}

void DrawingRawShaderEffect_Vulkan::LoadShaderInfo(const std::shared_ptr<DrawingRawShader_Vulkan> pShader, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount)
{
	size_t wordCount = pShader->m_rawCode.size() * sizeof(char) / sizeof(uint32_t);
	assert(wordCount > 0);
	std::vector<uint32_t> rawCode(wordCount);
	memcpy(rawCode.data(), pShader->m_rawCode.data(), pShader->m_rawCode.size());

	spirv_cross::Compiler comp(move(rawCode));

	// Query only active variables
	auto active = comp.get_active_interface_variables();
	spirv_cross::ShaderResources res = comp.get_shader_resources(active);
	comp.set_enabled_interface_variables(move(active));

	LoadUniformBufferFromShader(comp, res, shaderType, maxSetsCount);
	LoadStorageBufferFromShader(comp, res, shaderType, maxSetsCount);
	LoadSubpassInputFromShader(comp, res, shaderType, maxSetsCount);
	LoadStorageImageFromShader(comp, res, shaderType, maxSetsCount);
	LoadCombinedImageSamplerFromShader(comp, res, shaderType, maxSetsCount);
	LoadSampledImageFromShader(comp, res, shaderType, maxSetsCount);
	LoadSamplerFromShader(comp, res, shaderType, maxSetsCount);
	LoadAccelerationStructureFromShader(comp, res, shaderType, maxSetsCount);
	LoadPushConstantFromShader(comp, res, shaderType);

	DrawingDevice::ConstBufferPropTable cbPropTable;
	BuildUBPropTable(cbPropTable, pShader.get());
	BindUniformBuffer(cbPropTable, pShader.get(), shaderType);
	GenerateParameters(pShader.get(), shaderType);
	BuildTextureTable(pShader.get(), shaderType);
}

void DrawingRawShaderEffect_Vulkan::LoadUniformBufferFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount)
{
	uint32_t counter = 0;

	for (const spirv_cross::Resource& resource : res.uniform_buffers)
	{
		VkDescriptorSetLayoutBinding binding = {};
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		binding.binding = comp.get_decoration(resource.id, spv::DecorationBinding);
		binding.stageFlags = VK_DetermineShaderStage(shaderType);
		binding.pImmutableSamplers = nullptr;

		m_layoutBindings.push_back(binding);
		counter++;
	}
	if (counter > 0)
	{
		VkDescriptorPoolSize poolSize = {};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = maxSetsCount * counter; // Alert: this could be incorrect
		m_poolSizes.push_back(poolSize);
	}
}
void DrawingRawShaderEffect_Vulkan::LoadStorageBufferFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount)
{
	uint32_t counter = 0;

	for (const spirv_cross::Resource& resource : res.storage_buffers)
	{
		VkDescriptorSetLayoutBinding binding;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		binding.binding = comp.get_decoration(resource.id, spv::DecorationBinding);
		binding.stageFlags = VK_DetermineShaderStage(shaderType);
		binding.pImmutableSamplers = nullptr;

		m_layoutBindings.push_back(binding);
		counter++;
	}
	if (counter > 0)
	{
		VkDescriptorPoolSize poolSize;
		poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		poolSize.descriptorCount = maxSetsCount * counter;
		m_poolSizes.push_back(poolSize);
	}
}

void DrawingRawShaderEffect_Vulkan::LoadSubpassInputFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount)
{
	uint32_t counter = 0;

	for (const spirv_cross::Resource& resource : res.subpass_inputs)
	{
		VkDescriptorSetLayoutBinding binding;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT; // Alert: this interpretation may be incorrect
		binding.binding = comp.get_decoration(resource.id, spv::DecorationBinding);
		binding.stageFlags = VK_DetermineShaderStage(shaderType);
		binding.pImmutableSamplers = nullptr;

		m_layoutBindings.push_back(binding);
		counter++;
		// This attachment index info has not been used
		//unsigned attachment_index = comp.get_decoration(resource.id, spv::DecorationInputAttachmentIndex);
	}
	if (counter > 0)
	{
		VkDescriptorPoolSize poolSize;
		poolSize.type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		poolSize.descriptorCount = maxSetsCount * counter;
		m_poolSizes.push_back(poolSize);
	}
}

void DrawingRawShaderEffect_Vulkan::LoadStorageImageFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount)
{
	uint32_t counter = 0;

	for (const spirv_cross::Resource& resource : res.storage_images)
	{
		VkDescriptorSetLayoutBinding binding;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		binding.binding = comp.get_decoration(resource.id, spv::DecorationBinding);
		binding.stageFlags = VK_DetermineShaderStage(shaderType);
		binding.pImmutableSamplers = nullptr;

		m_layoutBindings.push_back(binding);
		counter++;
	}
	if (counter > 0)
	{
		VkDescriptorPoolSize poolSize;
		poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		poolSize.descriptorCount = maxSetsCount * counter;
		m_poolSizes.push_back(poolSize);
	}
}

void DrawingRawShaderEffect_Vulkan::LoadCombinedImageSamplerFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount)
{
	uint32_t counter = 0;

	for (const spirv_cross::Resource& resource : res.sampled_images)
	{
		VkDescriptorSetLayoutBinding binding;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		binding.binding = comp.get_decoration(resource.id, spv::DecorationBinding);
		binding.stageFlags = VK_DetermineShaderStage(shaderType);
		binding.pImmutableSamplers = nullptr;

		m_layoutBindings.push_back(binding);
		counter++;
		// This set index info has not been used yet
		//unsigned set = comp.get_decoration(resource.id, spv::DecorationDescriptorSet);
	}
	if (counter > 0)
	{
		VkDescriptorPoolSize poolSize;
		poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSize.descriptorCount = maxSetsCount * counter;
		m_poolSizes.push_back(poolSize);
	}
}

void DrawingRawShaderEffect_Vulkan::LoadAccelerationStructureFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount)
{
	// I don't know where this would be used for yet

	uint32_t counter = 0;

	for (const spirv_cross::Resource& resource : res.acceleration_structures)
	{
		VkDescriptorSetLayoutBinding binding;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;
		binding.binding = comp.get_decoration(resource.id, spv::DecorationBinding);
		binding.stageFlags = VK_DetermineShaderStage(shaderType);
		binding.pImmutableSamplers = nullptr;

		m_layoutBindings.push_back(binding);
		counter++;
	}
	if (counter > 0)
	{
		VkDescriptorPoolSize poolSize;
		poolSize.type = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;
		poolSize.descriptorCount = maxSetsCount * counter;
		m_poolSizes.push_back(poolSize);
	}
}

void DrawingRawShaderEffect_Vulkan::LoadSampledImageFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount)
{
	uint32_t counter = 0;

	for (const spirv_cross::Resource& resource : res.separate_images)
	{
		VkDescriptorSetLayoutBinding binding;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		binding.binding = comp.get_decoration(resource.id, spv::DecorationBinding);
		binding.stageFlags = VK_DetermineShaderStage(shaderType);
		binding.pImmutableSamplers = nullptr;

		m_layoutBindings.push_back(binding);
		counter++;
	}
	if (counter > 0)
	{
		VkDescriptorPoolSize poolSize;
		poolSize.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		poolSize.descriptorCount = maxSetsCount * counter;
		m_poolSizes.push_back(poolSize);
	}
}

void DrawingRawShaderEffect_Vulkan::LoadSamplerFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount)
{
	uint32_t counter = 0;

	for (const spirv_cross::Resource& resource : res.separate_samplers)
	{
		VkDescriptorSetLayoutBinding binding;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		binding.binding = comp.get_decoration(resource.id, spv::DecorationBinding);
		binding.stageFlags = VK_DetermineShaderStage(shaderType);
		binding.pImmutableSamplers = nullptr;

		m_layoutBindings.push_back(binding);
		counter++;
	}
	if (counter > 0)
	{
		VkDescriptorPoolSize poolSize;
		poolSize.type = VK_DESCRIPTOR_TYPE_SAMPLER;
		poolSize.descriptorCount = maxSetsCount * counter;
		m_poolSizes.push_back(poolSize);
	}
}

void DrawingRawShaderEffect_Vulkan::LoadPushConstantFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType)
{
	for (const spirv_cross::Resource& resource : res.push_constant_buffers)
	{
		VkPushConstantRange range;
		range.offset = comp.get_decoration(resource.id, spv::DecorationOffset);
		range.size = static_cast<uint32_t>(comp.get_declared_struct_size(comp.get_type(resource.base_type_id)));
		range.stageFlags = VK_DetermineShaderStage(shaderType);

		m_pushConstantRanges.push_back(range);
	}
}

bool DrawingRawShaderEffect_Vulkan::CreateDescriptorSetLayout()
{
	m_pDescriptorSetLayout = std::make_shared<DrawingDescriptorSetLayout_Vulkan>(m_pDevice, m_layoutBindings);
	return true;
}

bool DrawingRawShaderEffect_Vulkan::CreateDescriptorPool(uint32_t maxSetsCount)
{
	auto pDescriptorAllocator = m_pDevice->GetDescriptorAllocator();
	m_descriptorPoolID = pDescriptorAllocator->CreateDescriptorPool(maxSetsCount, m_poolSizes);

	return m_descriptorPoolID != -1;
}

bool DrawingRawShaderEffect_Vulkan::AllocateDescriptorSets(uint32_t setsCount)
{
	assert(m_descriptorPoolID != -1);
	assert(static_cast<uint32_t>(m_descriptorSets.size()) + setsCount <= DrawingDevice_Vulkan::MAX_DESCRIPTOR_SETS_PER_EFFECT);

	std::vector<VkDescriptorSetLayout> layouts(setsCount, m_pDescriptorSetLayout->GetDescriptorSetLayout());

	auto pDescriptorAllocator = m_pDevice->GetDescriptorAllocator();
	pDescriptorAllocator->AllocateDescriptorSets(m_descriptorPoolID, layouts, m_descriptorSets);

	return true;
}

void DrawingRawShaderEffect_Vulkan::SParamVar::UpdateValues()
{
	assert(mpParam != nullptr);
	std::shared_ptr<DrawingRawConstantBuffer> pPrevCB = nullptr;
	for (auto index = 0; index < DrawingRawShader::RawShader_Count; ++index)
	{
		auto pCurCB = mpCB[index];
		if (pCurCB != nullptr && pCurCB != pPrevCB)
		{
			pCurCB->SetValue(mOffset[index], mpParam->GetValuePtr(), mpParam->GetValueSize());
			pPrevCB = pCurCB;
			mpParam->SetDirty(false);
		}
	}
}

void DrawingRawShaderEffect_Vulkan::UpdateParameterValues()
{
	for (auto& item : m_varTable)
	{
		item.second.UpdateValues();
	}
}

void DrawingRawShaderEffect_Vulkan::UpdateUniformBuffers()
{
	for (auto& item : m_uniformBufferTable)
	{
		auto& pUB = item.second.mpUB;
		if (pUB->IsDirty())
		{
			pUB->UpdateToHardware();
		}
	}
}

void DrawingRawShaderEffect_Vulkan::UpdateDescriptorSets()
{
	if (m_descriptorSetUpdateIndex >= m_descriptorSets.size())
	{
		AllocateDescriptorSets(1); // We can allocate more at once if we want to reduce the number of allocations
	}

	std::vector<VkDescriptorSet> descSets = { m_descriptorSets[m_descriptorSetUpdateIndex] };
	auto pCommandManager = m_pDevice->GetCommandManager(eCommandList_Direct);

	if (m_descriptorDirty)
	{
		std::unordered_map<uint32_t, std::vector<VkDescriptorBufferInfo>> uniformBufferBindingInfos; // (binding index, descriptor infos)
		std::unordered_map<uint32_t, std::vector<VkDescriptorImageInfo>>  sampledImageBindingInfos;

		// Uniform buffer
		for (size_t i = 0; i < m_pUniformBuffers.size(); ++i)
		{
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = m_pUniformBuffers[i]->GetUniformBufferImpl()->m_buffer;
			bufferInfo.offset = 0; // Alert: this offset could be incorrect if we have sub-buffer-division
			bufferInfo.range = VK_WHOLE_SIZE; //m_pUniformBuffers[i]->GetUniformBufferImpl()->GetSize();

			if (uniformBufferBindingInfos.find(m_pUniformBuffers[i]->GetBindingIndex()) == uniformBufferBindingInfos.end())
			{
				std::vector<VkDescriptorBufferInfo> uniformBufferInfos = { bufferInfo };
				uniformBufferBindingInfos.emplace(m_pUniformBuffers[i]->GetBindingIndex(), uniformBufferInfos);
			}
			else
			{
				uniformBufferBindingInfos[m_pUniformBuffers[i]->GetBindingIndex()].emplace_back(bufferInfo);
			}
		}

		// Sampled image
		m_pSampledImages.clear();
		for (auto& item : m_texTable)
		{
			auto& resDesc = item.second;

			uint32_t arraySize{ 0 };
			auto pTexArray = resDesc.mpParam->AsTextureArray(arraySize);
			assert(arraySize == 0 || arraySize == resDesc.mCount);

			for (uint32_t index = 0; index < resDesc.mCount; ++index)
			{
				auto pTex = static_cast<const DrawingRawTexture_Vulkan*>(pTexArray[index]);
				if (pTex == nullptr)
				{
					continue;
				}
#if defined(POOLED_TEXTURE_VK)
				auto texPool = m_pDevice->GetTexturePool(pTex->m_poolID);
				m_pSampledImages.emplace_back(std::make_pair(resDesc.mStartSlot[0], texPool->RequestTexture(texPool->LastRequestedTextureID()))); // Alert: we assume that the last requested texture is the one being updated
#else
				m_pSampledImages.emplace_back(std::make_pair(resDesc.mStartSlot[0], pTex)); // Alert: taking mStartSlot[0] won't always be correct
#endif
			}
		}
		for (size_t i = 0; i < m_pSampledImages.size(); ++i)
		{
			VkDescriptorImageInfo textureInfo = {};
			textureInfo.imageView = m_pSampledImages[i].second->GetResource()->m_imageView;
			textureInfo.imageLayout = m_pSampledImages[i].second->GetResource()->m_layout;
			textureInfo.sampler = VK_NULL_HANDLE; // Alert: sampler needs to be specified if a sampler is assigned

			if (sampledImageBindingInfos.find(m_pSampledImages[i].first) == sampledImageBindingInfos.end())
			{
				std::vector<VkDescriptorImageInfo> sampledImageInfos = { textureInfo };
				sampledImageBindingInfos.emplace(m_pSampledImages[i].first, sampledImageInfos);
			}
			else
			{
				sampledImageBindingInfos[m_pSampledImages[i].first].emplace_back(textureInfo);
			}
		}

		auto pDescriptorAllocator = m_pDevice->GetDescriptorAllocator();

		std::vector<DescriptorUpdateInfo_Vulkan> updateInfos;
		updateInfos.resize(uniformBufferBindingInfos.size() + sampledImageBindingInfos.size()); // Two types of update info are handled

		size_t currIndex = 0;

		// Uniform buffer
		for (auto& itr = uniformBufferBindingInfos.begin(); itr != uniformBufferBindingInfos.end(); ++itr)
		{
			updateInfos[currIndex].TargetDescriptorSet = m_descriptorSets[m_descriptorSetUpdateIndex];
			updateInfos[currIndex].TargetArrayElement = 0;
			updateInfos[currIndex].TargetDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			updateInfos[currIndex].InfoType = eDescriptorInfo_Buffer;
			updateInfos[currIndex].BufferInfos = itr->second;
			updateInfos[currIndex].HasContent = itr->second.size() > 0 ? true : false;
			updateInfos[currIndex].TargetDescriptorBinding = itr->first;
			currIndex++;
		}

		// Sampled image
		for (auto& itr = sampledImageBindingInfos.begin(); itr != sampledImageBindingInfos.end(); ++itr)
		{
			updateInfos[currIndex].TargetDescriptorSet = m_descriptorSets[m_descriptorSetUpdateIndex];
			updateInfos[currIndex].TargetArrayElement = 0;
			updateInfos[currIndex].TargetDescriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			updateInfos[currIndex].InfoType = eDescriptorInfo_Image;
			updateInfos[currIndex].ImageInfos = itr->second;
			updateInfos[currIndex].HasContent = itr->second.size() > 0 ? true : false;
			updateInfos[currIndex].TargetDescriptorBinding = itr->first;
			currIndex++;
		}

		pDescriptorAllocator->UpdateDescriptorSets(updateInfos);

		m_descriptorDirty = false;	

#if defined(_VK_DEBUG_MSG)
		std::cout << "Descriptor sets updated" << std::endl;
#endif
	}

	m_descriptorSetUpdateIndex += 1;

	assert(pCommandManager->IsCommandBufferInUse(m_pDevice->t_recordingCmdBufferID));
	pCommandManager->BindDescriptorSets(m_pDevice->t_recordingCmdBufferID, VK_PIPELINE_BIND_POINT_GRAPHICS, descSets);
}

void DrawingRawShaderEffect_Vulkan::ResetDescriptorUpdateIndex()
{
	m_descriptorSetUpdateIndex = 0;
}

DrawingRawPipelineState_Vulkan::DrawingRawPipelineState_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<DrawingRawEffect_Vulkan> pEffect, VkGraphicsPipelineCreateInfo& pipelineInfo)
	: m_pDevice(pDevice), m_pEffect(pEffect)
{
	m_pipeline = VK_NULL_HANDLE;

	if (!VK_CheckResult(vkCreateGraphicsPipelines(pDevice->m_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline), "vkCreateGraphicsPipelines"))
	{
		throw std::runtime_error("Failed to create graphics pipeline.");
	}
}

DrawingRawPipelineState_Vulkan::~DrawingRawPipelineState_Vulkan()
{
	vkDestroyPipeline(m_pDevice->m_logicalDevice, m_pipeline, nullptr);
}

VkPipeline DrawingRawPipelineState_Vulkan::GetPipeline() const
{
	return m_pipeline;
}

std::shared_ptr<DrawingRawEffect_Vulkan> DrawingRawPipelineState_Vulkan::GetEffect() const
{
	return m_pEffect;
}

DrawingRawUniformBuffer_Vulkan::DrawingRawUniformBuffer_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, uint32_t sizeInBytes, uint32_t layoutParameterIndex) :
	DrawingRawConstantBuffer(sizeInBytes), m_pDevice(pDevice), m_layoutParameterIndex(layoutParameterIndex), m_eType(eUniformBuffer_UNDEFINED), m_appliedShaderStage(VK_SHADER_STAGE_VERTEX_BIT), m_memoryMapped(false)
{
	m_pData = new char[sizeInBytes];
	memset(m_pData, 0, sizeof(char) * sizeInBytes);
}

DrawingRawUniformBuffer_Vulkan::~DrawingRawUniformBuffer_Vulkan()
{
	if (m_memoryMapped)
	{
		m_pAllocator->UnmapMemory(m_pBufferImpl->m_allocation);
	}
}

void Engine::DrawingRawUniformBuffer_Vulkan::UpdateToHardware() // Q&A: Why we have to specify Engine:: here
{
	assert(m_pDevice != nullptr);
	assert(m_pData != nullptr);

	auto pCommandManager = m_pDevice->GetCommandManager(eCommandList_Direct);

	switch (m_eType)
	{
	case eUniformBuffer_PUSH_CONSTANT:
		assert(pCommandManager->IsCommandBufferInUse(m_pDevice->t_recordingCmdBufferID));
		pCommandManager->UpdatePushConstant(m_pDevice->t_recordingCmdBufferID, m_appliedShaderStage, m_sizeInBytes, m_pData);
		break;

	case eUniformBuffer_UNIFORM:
		if (m_pBufferImpl == nullptr)
		{
			throw std::runtime_error("Vulkan uniform buffer not initialized.");
			return;
		}
		if (!m_memoryMapped)
		{
			m_memoryMapped = m_pAllocator->MapMemory(m_pBufferImpl->m_allocation, &m_hostData);
		}
		memcpy(m_hostData, m_pData, m_sizeInBytes);
		// Alert: since we are using persistent buffer mappings for frequent updates, be careful for memory leaks
		break;

	default:
		throw std::runtime_error("Undefined Vulkan uniform buffer.");
		break;
	}
}

void DrawingRawUniformBuffer_Vulkan::MarkAsUniformBuffer()
{
	if (m_pBufferImpl != nullptr)
	{
		throw std::runtime_error("This Vulkan raw buffer has already been assigned as uniform buffer.");
		return;
		// Comment: or maybe we could try free and re-create uniform buffer here
	}

	m_pAllocator = m_pDevice->GetUploadAllocator();
	m_pBufferImpl = std::make_shared<DrawingRawBuffer_Vulkan>(m_pDevice, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, m_sizeInBytes);
	m_eType = eUniformBuffer_UNIFORM;
}

void DrawingRawUniformBuffer_Vulkan::MarkAsPushConstant(VkShaderStageFlags appliedStage)
{
	m_appliedShaderStage = appliedStage;
	m_eType = eUniformBuffer_PUSH_CONSTANT;
}

void DrawingRawUniformBuffer_Vulkan::UnmarkPushConstant()
{
	m_eType = (m_pBufferImpl != nullptr) ? eUniformBuffer_UNIFORM : eUniformBuffer_UNDEFINED;
}

const UniformBufferType_Vulkan DrawingRawUniformBuffer_Vulkan::GetType() const
{
	return m_eType;
}

std::shared_ptr<DrawingRawBuffer_Vulkan> DrawingRawUniformBuffer_Vulkan::GetUniformBufferImpl() const
{
	return m_pBufferImpl;
}

const uint32_t DrawingRawUniformBuffer_Vulkan::GetBindingIndex() const
{
	return m_layoutParameterIndex;
}

DrawingRawTexture_Vulkan::DrawingRawTexture_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const RawImageCreateInfo_Vulkan& createInfo, std::vector<VulkanSubresourceData>& data, uint32_t dataSize)
	: m_pDevice(pDevice), m_layoutParameterIndex(-1)
{
	m_pResource = std::make_shared<DrawingRawImage_Vulkan>(pDevice, createInfo);
#if defined(POOLED_TEXTURE_VK)
	m_copyFinishedSemaphoreID = -1;
	m_poolID = -1;
#endif

	auto pCommandManager = m_pDevice->GetCommandManager(eCommandList_Direct);
	pCommandManager->TransitionImageLayout_Immediate(m_pResource, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	CopySubresource(data, dataSize, true);
}

std::shared_ptr<DrawingRawImage_Vulkan> DrawingRawTexture_Vulkan::GetResource() const
{
	return m_pResource;
}

void DrawingRawTexture_Vulkan::CopySubresource(const std::vector<VulkanSubresourceData>& data, uint32_t dataSize, const bool immediateCopy, const bool readAfterCopy)
{
	// Alert: we only use one level of mipmap right now, so we overwrite the dataSize. Should mipmapping be enabled in the future, this part needs to be removed
	dataSize = data[0].SliceSize;

#if defined(POOLED_STAGING_BUFFER_VK)
	auto pStagingBufferPool = m_pDevice->GetStagingBufferPool();
	uint32_t stagingBufferID = pStagingBufferPool->RequestStagingBufferID(dataSize);
	auto pStagingBuffer = pStagingBufferPool->RequestStagingBuffer(stagingBufferID);
#else
	VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_CPU_ONLY;
	auto pStagingBuffer = std::make_shared<DrawingRawBuffer_Vulkan>(m_pDevice, usage, memoryUsage, dataSize);
#endif

	// We use single staging buffer with regions to copy into different image mipmap levels
	std::vector<VkBufferImageCopy> regions;

	auto pAllocator = m_pResource->GetAllocator();
	if (pAllocator == nullptr)
	{
		pAllocator = m_pDevice->GetUploadAllocator();
	}

	for (uint32_t level = 0; level < m_pResource->m_mipLevels; ++level)
	{
		void* subData;
		// Alert: we only use one level of mipmap right now
		//vkMapMemory(m_pDevice->m_logicalDevice, stagingBuffer.m_allocation->GetMemory(), data[level].Offset, data[level].SliceSize, 0, &subData);
		pAllocator->MapMemory(pStagingBuffer->m_allocation, &subData);
		memcpy(subData, data[level].pData, static_cast<size_t>(data[level].SliceSize));
		pAllocator->UnmapMemory(pStagingBuffer->m_allocation);
		//vkUnmapMemory(m_pDevice->m_logicalDevice, stagingBuffer.m_allocation->GetMemory());

		VkBufferImageCopy region = {};
		region.bufferOffset = data[level].Offset;
		region.bufferRowLength = 0; // Tightly packed
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Alert: this usage should be modified according to actual texture purpose
		region.imageSubresource.mipLevel = level;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1; // We assume there is only one layer for now (equivalent to desc.mArraySize = 1 ?)
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { static_cast<uint32_t>(data[level].RowSize / data[level].FormatBytes), static_cast<uint32_t>(data[level].SliceSize / data[level].RowSize), 1 };
		regions.push_back(region);
	}

	auto pDirectCommandManager = m_pDevice->GetCommandManager(eCommandList_Direct);
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	auto pTransferCommandManager = m_pDevice->GetCommandManager(eCommandList_Copy);
#endif

	VkImageLayout originalLayout = readAfterCopy ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : m_pResource->m_layout;
	if (!immediateCopy)
	{
		// TODO: This part is too ugly, needs clean up

		if (!pDirectCommandManager->IsCommandBufferInUse(m_pDevice->t_recordingCmdBufferID))
		{
			m_pDevice->t_recordingCmdBufferID = pDirectCommandManager->BeginNewCommandBuffer();
		}

#if defined(ENABLE_TRANSFER_QUEUE_VK)
		auto pSyncObjectManager = m_pDevice->GetSyncObjectManager();

		if (!pTransferCommandManager->IsCommandBufferInUse(m_pDevice->t_recordingTransCmdBufferID))
		{
			m_pDevice->t_recordingTransCmdBufferID = pTransferCommandManager->BeginNewCommandBuffer();
		}

#ifndef POOLED_TEXTURE_VK
		// Vulkan's transfer queue only supports VK_PIPELINE_STAGE_TRANSFER_BIT, so we have to perform layout transition on graphics queue
		pDirectCommandManager->TransitionImageLayout_Recorded(m_pDevice->t_recordingCmdBufferID, m_pResource, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		uint32_t transFinishedSemaphoreID = pSyncObjectManager->RequestSemaphoreID();
		
		std::vector<std::shared_ptr<SignalSempahore_Vulkan>> transFinishedSemaphores = { std::make_shared<SignalSempahore_Vulkan>(transFinishedSemaphoreID, pSyncObjectManager->RequestSemaphore(transFinishedSemaphoreID)) };
		pDirectCommandManager->SubmitCommandBuffers(VK_NULL_HANDLE, transFinishedSemaphores);

		pTransferCommandManager->AddSemaphoreToWaitlist(std::make_shared<WaitSemaphore_Vulkan>(transFinishedSemaphoreID, pSyncObjectManager->RequestSemaphore(transFinishedSemaphoreID), VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT));
		if (m_pDevice->t_renderFinishedSemaphoreID != -1)
		{
			pTransferCommandManager->AddSemaphoreToWaitlist(std::make_shared<WaitSemaphore_Vulkan>(m_pDevice->t_renderFinishedSemaphoreID, pSyncObjectManager->RequestSemaphore(m_pDevice->t_renderFinishedSemaphoreID), VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT));
		}
#endif

#elif defined(SUBMIT_COMMAND_PER_DRAW_VK)
		pDirectCommandManager->TransitionImageLayout_Recorded(m_pDevice->t_recordingCmdBufferID, m_pResource, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

#if defined(ENABLE_TRANSFER_QUEUE_VK)
		auto pSyncObjectManager = m_pDevice->GetSyncObjectManager();
		if (m_pDevice->t_renderFinishedSemaphoreID != -1)
		{
			pDirectCommandManager->AddSemaphoreToWaitlist(std::make_shared<WaitSemaphore_Vulkan>(m_pDevice->t_renderFinishedSemaphoreID, pSyncObjectManager->RequestSemaphore(m_pDevice->t_renderFinishedSemaphoreID), VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT));
		}
#endif
#else
		pDirectCommandManager->TransitionImageLayout_Recorded(m_pDevice->t_recordingCmdBufferID, m_pResource, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
#endif

		pAllocator->CopyBufferToImage_Recorded(pStagingBuffer, m_pResource->m_image, regions);

#if defined(ENABLE_TRANSFER_QUEUE_VK)
#ifndef CONTROLLED_SUBMIT_RATE_VK
		uint32_t copyFinishedSemaphoreID = pSyncObjectManager->RequestSemaphoreID();

#if defined(POOLED_TEXTURE_VK)
		m_copyFinishedSemaphoreID = copyFinishedSemaphoreID; // Alert: since we are cancelling some semaphores when CONTROLLED_SUBMIT_RATE_VK enabled, using m_copyFinishedSemaphoreID could be incorrect
#endif
		pDirectCommandManager->AddSemaphoreToWaitlist(std::make_shared<WaitSemaphore_Vulkan>(copyFinishedSemaphoreID, pSyncObjectManager->RequestSemaphore(copyFinishedSemaphoreID), VK_PIPELINE_STAGE_TRANSFER_BIT, false, eWaitType_Copy));

		std::vector<std::shared_ptr<SignalSempahore_Vulkan>> copyFinishedSemaphores = { std::make_shared<SignalSempahore_Vulkan>(copyFinishedSemaphoreID, pSyncObjectManager->RequestSemaphore(copyFinishedSemaphoreID), eWaitType_Copy) };
#else
		std::vector<std::shared_ptr<SignalSempahore_Vulkan>> copyFinishedSemaphores = {};
#endif
		pTransferCommandManager->SubmitCommandBuffers(VK_NULL_HANDLE, copyFinishedSemaphores);

		if (!pDirectCommandManager->IsCommandBufferInUse(m_pDevice->t_recordingCmdBufferID))
		{
			m_pDevice->t_recordingCmdBufferID = pDirectCommandManager->BeginNewCommandBuffer();
		}
#endif
		pDirectCommandManager->TransitionImageLayout_Recorded(m_pDevice->t_recordingCmdBufferID, m_pResource, originalLayout);
	}
	else
	{
		pDirectCommandManager->TransitionImageLayout_Immediate(m_pResource, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		pAllocator->CopyBufferToImage_Immediate(pStagingBuffer->m_buffer, m_pResource->m_image, regions);
		pDirectCommandManager->TransitionImageLayout_Immediate(m_pResource, originalLayout);
#if defined(POOLED_STAGING_BUFFER_VK)
		pStagingBufferPool->ReturnStagingBuffer(stagingBufferID);
#else
		pStagingBuffer->Free();
#endif
	}
}

void* DrawingRawTexture_Vulkan::MapResource()
{
	std::cout << "Vulkan: texture does not support direct mapping." << std::endl;
	return nullptr;
}

void DrawingRawTexture_Vulkan::UnmapResource()
{
	std::cout << "Vulkan: texture does not support direct unmapping." << std::endl;
}

DrawingRawTexture2D_Vulkan::DrawingRawTexture2D_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const RawImageCreateInfo_Vulkan& createInfo, std::vector<VulkanSubresourceData>& data, uint32_t dataSize)
	: DrawingRawTexture_Vulkan(pDevice, createInfo, data, dataSize)
{
	assert(createInfo.extent.depth <= 1);
}

#if defined(POOLED_TEXTURE_VK)
DrawingRawTexture_Vulkan::DrawingRawTexture_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const RawImageCreateInfo_Vulkan& createInfo)
	: m_pDevice(pDevice), m_layoutParameterIndex(-1), m_copyFinishedSemaphoreID(-1), m_poolID(-1)
{
	m_pResource = std::make_shared<DrawingRawImage_Vulkan>(pDevice, createInfo);

	auto pCommandManager = m_pDevice->GetCommandManager(eCommandList_Direct);
	pCommandManager->TransitionImageLayout_Immediate(m_pResource, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
}

DrawingRawTexturePool_Vulkan::DrawingRawTexturePool_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, uint32_t initialSize, const RawImageCreateInfo_Vulkan& textureTemplate, uint32_t poolID)
	: m_pDevice(pDevice), POOL_TEMPLATE(textureTemplate), m_distributedID(0), m_poolID(poolID)
{
	AllocateNewPooledTexture(initialSize);
}

DrawingRawTexturePool_Vulkan::~DrawingRawTexturePool_Vulkan()
{
	FreeTextures();
}

const uint32_t DrawingRawTexturePool_Vulkan::GetPoolID() const
{
	return m_poolID;
}

const uint32_t DrawingRawTexturePool_Vulkan::RequestTextureID()
{
	if (m_distributedID >= static_cast<uint32_t>(m_pooledTextures.size()))
	{
		AllocateNewPooledTexture(1); // We can allocate more at once if we want to reduce allocate times
	}

	m_distributedID += 1;
	return m_distributedID - 1;
}

const uint32_t DrawingRawTexturePool_Vulkan::LastRequestedTextureID()
{
	return m_distributedID > 0 ? m_distributedID - 1 : 0;
}

const std::shared_ptr<DrawingRawTexture_Vulkan> DrawingRawTexturePool_Vulkan::RequestTexture(uint32_t id) const
{
	assert(id < static_cast<uint32_t>(m_pooledTextures.size()));
	return m_pooledTextures[id];
}

void DrawingRawTexturePool_Vulkan::ResetPool(bool deepReset)
{
	m_distributedID = 0;

	if (deepReset)
	{
		FreeTextures();
	}
}

bool DrawingRawTexturePool_Vulkan::TransitionPoolTextures(VkImageLayout destLayout)
{
	auto pCommandManager = m_pDevice->GetCommandManager(eCommandList_Direct);
	DrawingCommandBuffer_Vulkan cmdBuffer = pCommandManager->BeginSingleTimeCommands();

	for (auto& texture : m_pooledTextures)
	{
		pCommandManager->TransitionImageLayout_Recorded(cmdBuffer, texture->GetResource(), destLayout);
	}

	pCommandManager->EndSingleTimeCommands(cmdBuffer);
	return true;
}

bool DrawingRawTexturePool_Vulkan::FreeTextures()
{
	for (auto& texture : m_pooledTextures)
	{
		texture->GetResource()->Free();
	}
	m_pooledTextures.clear();
	return true;
}

bool DrawingRawTexturePool_Vulkan::AllocateNewPooledTexture(uint32_t count)
{
	assert((m_pooledTextures.size() + count) <= MAX_POOL_SIZE);

	for (uint32_t i = 0; i < count; ++i)
	{
		m_pooledTextures.emplace_back(std::make_shared<DrawingRawTexture_Vulkan>(m_pDevice, POOL_TEMPLATE));
		m_pooledTextures[m_pooledTextures.size() - 1]->m_poolID = m_poolID;
	}

	return true;
}
#endif

#if defined(POOLED_STAGING_BUFFER_VK)
DrawingRawStagingBufferPool_Vulkan::DrawingRawStagingBufferPool_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice)
	: m_pDevice(pDevice)
{
}

DrawingRawStagingBufferPool_Vulkan::~DrawingRawStagingBufferPool_Vulkan()
{
	for (auto& buffer : m_stagingBuffers)
	{
		buffer->Free();
	}
}

uint32_t DrawingRawStagingBufferPool_Vulkan::RequestStagingBufferID(uint32_t desiredSize)
{
	for (auto& buffer : m_bufferAvailability)
	{
		if (buffer.second && m_stagingBuffers[buffer.first]->GetSize() == desiredSize)
		{
			buffer.second = false;
			return buffer.first;
		}
	}

	assert(m_stagingBuffers.size() < MAX_POOL_SIZE);

	m_stagingBuffers.emplace_back(std::make_shared<DrawingRawBuffer_Vulkan>(m_pDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, desiredSize));
	m_stagingBuffers[m_stagingBuffers.size() - 1]->m_stagingID = static_cast<uint32_t>(m_stagingBuffers.size() - 1);
	m_bufferAvailability.emplace(m_stagingBuffers.size() - 1, false);
	return static_cast<uint32_t>(m_stagingBuffers.size() - 1);
}

std::shared_ptr<DrawingRawBuffer_Vulkan> DrawingRawStagingBufferPool_Vulkan::RequestStagingBuffer(uint32_t id) const
{
	assert(id < m_stagingBuffers.size());
	return m_stagingBuffers[id];
}

void DrawingRawStagingBufferPool_Vulkan::ReturnStagingBuffer(uint32_t id)
{
	assert(m_bufferAvailability.find(id) != m_bufferAvailability.end());
	m_bufferAvailability[id] = true;
}
#endif