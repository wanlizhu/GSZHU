#define NOMINMAX
//#define _VK_DEBUG_MSG // This significantly reduces performace, only enable when call sequence debugging is desired
#include "DrawingDevice_Vulkan.h"
#include "DrawingRawResource_Vulkan.h"

#include <set>
#include <iostream>
#include <algorithm>


using namespace Engine;


DrawingDevice_Vulkan::DrawingDevice_Vulkan()
{
	m_physicalDevice = VK_NULL_HANDLE;
	m_logicalDevice = VK_NULL_HANDLE;
}

DrawingDevice_Vulkan::~DrawingDevice_Vulkan()
{
	Shutdown();
}

void DrawingDevice_Vulkan::Initialize()
{
#if defined(_DEBUG)
	std::cout << "Initializing Vulkan..." << std::endl;

#if defined(ENABLE_TRANSFER_QUEUE_VK)
	std::cout << "Transfer Queue: Enabled" << std::endl;
#else
	std::cout << "Transfer Queue: Disabled" << std::endl;
#endif

#if defined(SUBMIT_COMMAND_PER_DRAW_VK)
	std::cout << "Command Submit Mode: Per Draw Call" << std::endl;
#else
	std::cout << "Command Submit Mode: Per Frame" << std::endl;
#endif
#endif
	assert(m_logicalDevice != VK_NULL_HANDLE && m_physicalDevice != VK_NULL_HANDLE && m_surface != VK_NULL_HANDLE);

	SetupCommandManager();
	SetupUploadAllocator();
	SetupDescriptorAllocator();

	// Acquire legacy sync object IDs
	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		m_imageAvailableSemaphoreIDs.emplace_back(m_pSyncObjectManager->RequestSemaphoreID());
		m_fenceIDs.emplace_back(m_pSyncObjectManager->RequestFenceID());
	}

#if defined(POOLED_STAGING_BUFFER_VK)
	m_pStagingBufferPool = std::make_shared<DrawingRawStagingBufferPool_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()));
#endif

	m_isRunning = true;
}

void DrawingDevice_Vulkan::Shutdown()
{
	if (!m_isRunning)
	{
		return;
	}

	CleanUpSwapchain();

	m_pDirectCommandManager->Destroy();
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	m_pTransferCommandManager->Destroy();
#endif
	m_pDescriptorAllocator->Destroy();

	// TODO: Destroy created textures & samplers

#if defined(POOLED_TEXTURE_VK)
	for (auto& texPool : m_texturePools)
	{
		texPool.second->ResetPool(true);
	}
#endif

	vkDestroyDevice(m_logicalDevice, nullptr);

	if (m_enableValidationLayers)
	{
		DestroyDebugUtilMessengerEXT_Vulkan(m_instance, &m_debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vkDestroyInstance(m_instance, nullptr);

	m_desiredExtensions.clear();
	m_availableExtensions.clear();

	m_isRunning = false;
}

bool DrawingDevice_Vulkan::CreateTarget(const DrawingTargetDesc& desc, std::shared_ptr<DrawingTarget>& pRes)
{
	std::shared_ptr<DrawingRawTarget> pTargetRaw = nullptr;
	if (desc.mHwnd == nullptr)
	{
		// Create non-swapchain target here
	}
	else
	{
		SwapchainSupportDetails_Vulkan swapchainSupport = QuerySwapchainSupport_Vulkan(m_physicalDevice, m_surface);
		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapchainSupport.formats);
		
		VkFormat descFormat = VK_FormatEnum(desc.mFormat);
		for (VkSurfaceFormatKHR supportedFormat : swapchainSupport.formats) // Alert: this is a hard coded fix, the passed-in format is incompatible with Q4000
		{
			if (descFormat == supportedFormat.format)
			{
				surfaceFormat.format = descFormat;
				break;
			}
		}
		
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapchainSupport.presentModes);
		VkExtent2D maxExtent = ChooseSwapExtent(swapchainSupport.capabilities);
		VkExtent2D extent = { std::min(desc.mWidth, maxExtent.width), std::min(desc.mHeight, maxExtent.height) };

		SwapchainCreateInfo_Vulkan createInfo;
		createInfo.swapchainSupport = swapchainSupport;
		createInfo.queueFamilyIndices = m_queueFamilyIndices;
		createInfo.surface = m_surface;
		createInfo.surfaceFormat = surfaceFormat;
		createInfo.presentMode = presentMode;
		createInfo.swapExtent = extent;
		createInfo.maxFramesInFlight = MAX_FRAMES_IN_FLIGHT;
	
		pTargetRaw = std::make_shared<DrawingRawSwapchain_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()), createInfo);
		
		// TODO: move these function calls to better place
		t_pSwapchain = std::static_pointer_cast<DrawingRawSwapchain_Vulkan>(pTargetRaw);
		t_pSwapchain->UpdateBackBuffer(m_pSyncObjectManager->RequestSemaphore(m_imageAvailableSemaphoreIDs[m_currentFrame]), std::numeric_limits<uint64_t>::max());
		t_newlyAcquiredImage = true;	
		PrepareRenderPasses();
	}

	auto pTarget = std::make_shared<DrawingTarget>(shared_from_this());
	pTarget->SetDesc(std::shared_ptr<DrawingResourceDesc>(desc.Clone()));
	pTarget->SetResource(pTargetRaw);

	pRes = pTarget;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreateTarget [Partial]" << std::endl;
#endif
	return true;
}

void DrawingDevice_Vulkan::ClearTarget(std::shared_ptr<DrawingTarget> pTarget, const float4& color)
{
	t_awaitClearTarget = true;
	t_clearColor = color;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan ClearTarget [Deferred]" << std::endl;
	std::cout << "Clearing render target will not be performed explicitly by the ClearTarget call." << std::endl;
#endif
}

void DrawingDevice_Vulkan::ClearDepthBuffer(std::shared_ptr<DrawingDepthBuffer> pDepthBuffer, float depth, uint8_t stencil, uint32_t flag)
{
	//std::cout << "Vulkan ClearDepthBuffer" << std::endl;
}

void DrawingDevice_Vulkan::SetVertexFormat(std::shared_ptr<DrawingVertexFormat> pFormat)
{
}

void DrawingDevice_Vulkan::SetVertexBuffer(std::shared_ptr<DrawingVertexBuffer> pVB[], uint32_t count)
{
	if (!m_pDirectCommandManager->IsCommandBufferInUse(t_recordingCmdBufferID))
	{
		t_recordingCmdBufferID = m_pDirectCommandManager->BeginNewCommandBuffer();
	}

	std::shared_ptr<DrawingRawVertexBuffer_Vulkan> pVertexBuffersRaw[MAX_VERTEX_STREAM] = { nullptr };
	for (uint32_t index = 0; index < count; ++index)
	{
		if (pVB[index] != nullptr)
			pVertexBuffersRaw[index] = std::dynamic_pointer_cast<DrawingRawVertexBuffer_Vulkan>(pVB[index]->GetResource());
		else
			pVertexBuffersRaw[index] = nullptr;
	}

	assert(count < m_deviceProperties.limits.maxVertexInputAttributes);

	std::vector<VkBuffer> buffers;
	std::vector<VkDeviceSize> offsets;
	uint32_t numBuffer = 0;
	for (uint32_t index = 0; index < count; ++index)
	{
		if (pVertexBuffersRaw[index] != nullptr)
		{
			buffers.push_back(pVertexBuffersRaw[index]->GetBuffer());
			offsets.push_back(0); // Alert: this offset could be wrong if there are multiple buffers
			numBuffer++;
		}
	}

	assert(numBuffer > 0);

	m_pDirectCommandManager->BindVertexBuffer(t_recordingCmdBufferID, 0, numBuffer, buffers.data(), offsets.data());

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan SetVertexBuffer [Implemented]" << std::endl;
#endif
}

void DrawingDevice_Vulkan::SetIndexBuffer(std::shared_ptr<DrawingIndexBuffer> pIB)
{
	if (!m_pDirectCommandManager->IsCommandBufferInUse(t_recordingCmdBufferID))
	{
		t_recordingCmdBufferID = m_pDirectCommandManager->BeginNewCommandBuffer();
	}

	if (pIB != nullptr)
	{
		auto pIndexBuffersRaw = std::dynamic_pointer_cast<DrawingRawIndexBuffer_Vulkan>(pIB->GetResource());
		m_pDirectCommandManager->BindIndexBuffer(t_recordingCmdBufferID, pIndexBuffersRaw->GetBuffer(), 0, pIndexBuffersRaw->GetFormat());
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan SetIndexBuffer [Implemented]" << std::endl;
#endif
}

void DrawingDevice_Vulkan::UpdateTexture(std::shared_ptr<DrawingTexture> pTex, const void* pData[], uint32_t size[], uint32_t slices)
{
	std::vector<VulkanSubresourceData> subResData(slices);

	auto pTextureRaw = std::dynamic_pointer_cast<DrawingRawTexture_Vulkan>(pTex->GetResource());
	auto pDesc = std::dynamic_pointer_cast<DrawingTextureDesc>(pTex->GetDesc());
	uint32_t dataSize = 0;
	if (!subResData.empty())
	{
		auto mipLevels = slices / pDesc->mArraySize;
		assert(mipLevels == pDesc->mMipLevels);
		for (uint32_t index = 0; index < pDesc->mArraySize; ++index)
		{
			auto bytesPerRow = pDesc->mBytesPerRow;
			auto bytesPerSlice = pDesc->mBytesPerSlice;
			for (uint32_t level = 0; level < mipLevels; ++level)
			{
				auto LOD = index * level + level;
				ZeroMemory(&subResData[LOD], sizeof(VulkanSubresourceData));
				subResData[LOD].pData = *(pData++);
				subResData[LOD].FormatBytes = VK_FormatBytes(pDesc->mFormat);
				subResData[LOD].RowSize = bytesPerRow;
				subResData[LOD].SliceSize = bytesPerSlice;
				subResData[LOD].Offset = dataSize;
				dataSize += size[LOD];
				bytesPerRow = bytesPerRow > 1U ? bytesPerRow >> 1 : 1U;
			}
		}

#if defined(POOLED_TEXTURE_VK)
		uint32_t texID = m_texturePools[pTextureRaw->m_poolID]->RequestTextureID();
		m_texturePools[pTextureRaw->m_poolID]->RequestTexture(texID)->CopySubresource(subResData, dataSize, false, true);
#else
		pTextureRaw->CopySubresource(subResData, dataSize);
#endif
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan UpdateTexture [Implemented]" << std::endl;
#endif
}

void DrawingDevice_Vulkan::SetBlendState(std::shared_ptr<DrawingBlendState> pBlend, float4 blendFactor, uint32_t sampleMask)
{
}

void DrawingDevice_Vulkan::SetDepthState(std::shared_ptr<DrawingDepthState> pDepth, uint32_t stencilRef)
{
}

void DrawingDevice_Vulkan::SetRasterState(std::shared_ptr<DrawingRasterState> pRaster)
{
}

void DrawingDevice_Vulkan::SetPipelineState(std::shared_ptr<DrawingPipelineState> pPipelineState)
{
	if (!m_pDirectCommandManager->IsCommandBufferInUse(t_recordingCmdBufferID))
	{
		t_recordingCmdBufferID = m_pDirectCommandManager->BeginNewCommandBuffer();
	}

	if (pPipelineState != nullptr)
	{
		auto pPipelineStateRaw = std::dynamic_pointer_cast<DrawingRawPipelineState_Vulkan>(pPipelineState->GetResource());
		assert(pPipelineStateRaw != nullptr);

		m_pDirectCommandManager->BindPipeline(t_recordingCmdBufferID, VK_PIPELINE_BIND_POINT_GRAPHICS, pPipelineStateRaw->GetPipeline());
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan SetPipelineState [Implemented]" << std::endl;
#endif
}

void DrawingDevice_Vulkan::PushBlendState()
{
}

void DrawingDevice_Vulkan::PopBlendState()
{
}

void DrawingDevice_Vulkan::PushDepthState()
{
}

void DrawingDevice_Vulkan::PopDepthState()
{
}

void DrawingDevice_Vulkan::PushRasterState()
{
}

void DrawingDevice_Vulkan::PopRasterState()
{
}

void DrawingDevice_Vulkan::SetViewport(Box2* vp)
{
#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan SetViewport [Deferred]" << std::endl;
#endif
}

void DrawingDevice_Vulkan::SetTargets(std::shared_ptr<DrawingTarget> pTarget[], uint32_t maxTargets, std::shared_ptr<DrawingDepthBuffer> pDepthBuffer, std::shared_ptr<DrawingRWBuffer> pRWBuffer[], uint32_t maxRWBuffers)
{
	if (!m_pDirectCommandManager->IsCommandBufferInUse(t_recordingCmdBufferID))
	{
		t_recordingCmdBufferID = m_pDirectCommandManager->BeginNewCommandBuffer();
	}

	// Alert: we assume there is only one target right now and the only target is current swapchain
	std::shared_ptr<DrawingRawFragmentTarget_Vulkan> pTargetsRaw[1] = { nullptr };

	if (maxTargets != 0)
	{
		std::transform(pTarget, pTarget + maxTargets, pTargetsRaw,
			[](std::shared_ptr<DrawingTarget> target) {
			return target != nullptr ? std::dynamic_pointer_cast<DrawingRawFragmentTarget_Vulkan>(target->GetResource()) : nullptr;
		});
	}

	// Alert: it may be incorrect to start render pass here
	VkFramebuffer framebuffer = std::static_pointer_cast<DrawingRawSwapchain_Vulkan>(pTargetsRaw[0])->GetTargetFramebuffer();
	VkExtent2D extent = std::static_pointer_cast<DrawingRawSwapchain_Vulkan>(pTargetsRaw[0])->GetExtent();
	if (t_awaitClearTarget)
	{
		m_pDirectCommandManager->BeginRenderPass(t_recordingCmdBufferID, m_renderPasses[0], framebuffer, extent, t_clearColor);
		t_awaitClearTarget = false;
	}
	else
	{
		m_pDirectCommandManager->BeginRenderPass(t_recordingCmdBufferID, m_renderPasses[1], framebuffer, extent);
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan SetTargets [Partial]" << std::endl;
#endif
}

bool DrawingDevice_Vulkan::UpdateEffectParameter(std::shared_ptr<DrawingParameter> pParam, std::shared_ptr<DrawingEffect> pEffect)
{
	assert(pEffect != nullptr);
	auto pRawEffect = std::dynamic_pointer_cast<DrawingRawEffect_Vulkan>(pEffect->GetResource());
	assert(pRawEffect != nullptr);

	pRawEffect->UpdateParameter(pParam);

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan UpdateEffectParameter [Implemented]" << std::endl;
#endif
	return true;
}

bool DrawingDevice_Vulkan::UpdateEffectTexture(std::shared_ptr<DrawingTexture> pTex, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect)
{
	assert(pTex != nullptr);
	assert(pEffect != nullptr);

	auto pRawEffect = std::dynamic_pointer_cast<DrawingRawEffect_Vulkan>(pEffect->GetResource());
	assert(pRawEffect != nullptr);

	auto pRawTex = std::dynamic_pointer_cast<DrawingRawTexture_Vulkan>(pTex->GetResource());
	assert(pRawTex != nullptr);

	auto pParamSet = pRawEffect->GetParameterSet();

	int32_t index = pParamSet.IndexOfName(pName);
	if (index < 0)
		return false;

	auto pParam = pParamSet[index];
	if (pParam == nullptr)
		return false;

	pParam->AsTexture(pRawTex.get());

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan UpdateEffectTexture [Implemented]" << std::endl;
#endif
	return true;
}

bool DrawingDevice_Vulkan::UpdateEffectBuffer(std::shared_ptr<DrawingTexBuffer> pBuffer, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect)
{
	std::cout << "Vulkan UpdateEffectBuffer" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::UpdateEffectSampler(std::shared_ptr<DrawingSamplerState> pSampler, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect)
{
	std::cout << "Vulkan UpdateEffectSampler" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::UpdateEffectTexBuffer(std::shared_ptr<DrawingTexBuffer> pTexBuffer, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect)
{
	std::cout << "Vulkan UpdateEffectTexBuffer" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::UpdateEffectRWBuffer(std::shared_ptr<DrawingRWBuffer> pRWBuffer, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect)
{
	std::cout << "Vulkan UpdateEffectRWBuffer" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::UpdateEffectInputRWBuffer(std::shared_ptr<DrawingRWBuffer> pRWBuffer, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect)
{
	std::cout << "Vulkan UpdateEffectInputRWBuffer" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::UpdateEffectOutputRWBuffer(std::shared_ptr<DrawingRWBuffer> pRWBuffer, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect)
{
	std::cout << "Vulkan UpdateEffectOutputRWBuffer" << std::endl;
	return false;
}

void DrawingDevice_Vulkan::BeginEffect(DrawingContext& dc, std::shared_ptr<DrawingEffect> pEffect)
{
	assert(pEffect != nullptr);

	auto pRawEffect = std::dynamic_pointer_cast<DrawingRawShaderEffect_Vulkan>(pEffect->GetResource());
	assert(pRawEffect != nullptr);

	// Alert: this is a stupid solution, we need a proper place to inform the effect that descriptors have been updated
	pRawEffect->DescriptorDirty();

	if (!m_pDirectCommandManager->IsCommandBufferInUse(t_recordingCmdBufferID))
	{
		t_recordingCmdBufferID = m_pDirectCommandManager->BeginNewCommandBuffer();
	}

	pRawEffect->Apply();

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan BeginEffect [Implemented]" << std::endl;
#endif
}

void DrawingDevice_Vulkan::EndEffect(DrawingContext& dc, std::shared_ptr<DrawingEffect> pEffect)
{
	assert(pEffect != nullptr);

	auto pRawEffect = std::dynamic_pointer_cast<DrawingRawEffect_Vulkan>(pEffect->GetResource());
	assert(pRawEffect != nullptr);

	pRawEffect->Terminate();

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan EndEffect [Implemented]" << std::endl;
#endif
}

bool DrawingDevice_Vulkan::CreateDepthBuffer(const DrawingDepthBufferDesc& desc, std::shared_ptr<DrawingDepthBuffer>& pRes)
{
	VkFormat depthFormat = FindDepthFormat();

	VkExtent3D imageExtent;
	imageExtent.width = t_pSwapchain->GetExtent().width;//desc.mWidth; // Error: the size parameter passed in by desc does not meet Vulkan swapchain limits
	imageExtent.height = t_pSwapchain->GetExtent().height;//desc.mHeight;
	imageExtent.depth = 1;

	RawImageCreateInfo_Vulkan createInfo;
	createInfo.extent = imageExtent;
	createInfo.format = depthFormat;
	createInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	createInfo.memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;
	createInfo.aspect = VK_IMAGE_ASPECT_DEPTH_BIT;

	std::shared_ptr<DrawingRawDepthTarget_Vulkan> pDepthTarget = std::make_shared<DrawingRawDepthTarget_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()), createInfo);
	m_pDirectCommandManager->TransitionImageLayout_Immediate(pDepthTarget->GetRenderTarget(), VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

	std::shared_ptr<DrawingRawTarget> pDepthBufferRaw = pDepthTarget;

	auto pTarget = std::make_shared<DrawingDepthBuffer>(shared_from_this());
	pTarget->SetDesc(std::shared_ptr<DrawingResourceDesc>(desc.Clone()));
	pTarget->SetResource(pDepthBufferRaw);

	pRes = pTarget;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreateDepthBuffer [Implemented]" << std::endl;
#endif

	// TODO: move this function call to better place
	t_pSwapchain->CreateFramebuffers(pDepthTarget->GetTargetView(), m_renderPasses[0]); // Our render passes are compatible with each other

	return true;
}

bool DrawingDevice_Vulkan::CreateBlendState(const DrawingBlendStateDesc& desc, std::shared_ptr<DrawingBlendState>& pRes)
{
	return true;
}

bool DrawingDevice_Vulkan::CreateDepthState(const DrawingDepthStateDesc& desc, std::shared_ptr<DrawingDepthState>& pRes)
{
	return true;
}

bool DrawingDevice_Vulkan::CreateRasterState(const DrawingRasterStateDesc& desc, std::shared_ptr<DrawingRasterState>& pRes)
{
	return true;
}

bool DrawingDevice_Vulkan::CreateSamplerState(const DrawingSamplerStateDesc& desc, std::shared_ptr<DrawingSamplerState>& pRes)
{
	std::cout << "Vulkan CreateSamplerState" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::CreateEffectFromFile(const DrawingEffectDesc& desc, std::shared_ptr<DrawingEffect>& pRes)
{
	std::cout << "Vulkan CreateEffectFromFile" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::CreateEffectFromString(const std::string& str, const DrawingEffectDesc& desc, std::shared_ptr<DrawingEffect>& pRes)
{
	std::cout << "Vulkan CreateEffectFromString" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::CreateEffectFromBuffer(const void* pData, uint32_t length, const DrawingEffectDesc& desc, std::shared_ptr<DrawingEffect>& pRes)
{
	std::cout << "Vulkan CreateEffectFromBuffer" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::CreateEffectFromShader(const DrawingEffectDesc& desc, std::shared_ptr<DrawingVertexShader> pVSShader, std::shared_ptr<DrawingPixelShader> pPSShader, std::shared_ptr<DrawingEffect>& pRes)
{
	assert(desc.mProgramType == eProgram_Shader);

	auto pVSShaderRaw = std::dynamic_pointer_cast<DrawingRawVertexShader_Vulkan>(pVSShader->GetResource());
	assert(pVSShaderRaw != nullptr);

	auto pFSShaderRaw = std::dynamic_pointer_cast<DrawingRawFragmentShader_Vulkan>(pPSShader->GetResource());
	assert(pFSShaderRaw != nullptr);

	auto pEffectRaw = std::make_shared<DrawingRawShaderEffect_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()), desc.mpName, pVSShaderRaw, pFSShaderRaw, MAX_DESCRIPTOR_SETS_PER_EFFECT);

	auto pEffect = std::make_shared<DrawingEffect>(shared_from_this());
	pEffect->SetDesc(std::shared_ptr<DrawingResourceDesc>(desc.Clone()));
	pEffect->SetResource(pEffectRaw);

	t_shaderEffects.emplace_back(pEffectRaw);

	pRes = pEffect;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreateEffectFromShader [Implemented]" << std::endl;
#endif
	return true;
}

bool DrawingDevice_Vulkan::CreateVertexShaderFromFile(const DrawingVertexShaderDesc& desc, std::shared_ptr<DrawingVertexShader>& pRes)
{
	assert(desc.mProgramType == eProgram_File);

	std::vector<char> rawCode;
	VkShaderModule shaderModule = VK_LoadSPIRVShader(desc.mpSourceName->c_str(), m_logicalDevice, true, rawCode);
	if (shaderModule == VK_NULL_HANDLE)
	{
		return false;
	}

	auto pVertexShaderRaw = std::make_shared<DrawingRawVertexShader_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()), desc.mpName, shaderModule, rawCode, (desc.mpEntryName.get())->c_str());
	auto pVertexShader = std::make_shared<DrawingVertexShader>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()));
	pVertexShader->SetDesc(std::shared_ptr<DrawingResourceDesc>(desc.Clone()));
	pVertexShader->SetResource(pVertexShaderRaw);

	pRes = pVertexShader;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreateVertexShaderFromFile [Implemented]" << std::endl;
#endif
	return true;
}

bool DrawingDevice_Vulkan::CreateVertexShaderFromString(const std::string& str, const DrawingVertexShaderDesc& desc, std::shared_ptr<DrawingVertexShader>& pRes)
{
	std::cout << "Vulkan CreateVertexShaderFromString" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::CreateVertexShaderFromBuffer(const void* pData, uint32_t length, const DrawingVertexShaderDesc& desc, std::shared_ptr<DrawingVertexShader>& pRes)
{
	std::cout << "Vulkan CreateVertexShaderFromBuffer" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::CreatePixelShaderFromFile(const DrawingPixelShaderDesc& desc, std::shared_ptr<DrawingPixelShader>& pRes)
{
	assert(desc.mProgramType == eProgram_File);

	std::vector<char> rawCode;
	VkShaderModule shaderModule = VK_LoadSPIRVShader(desc.mpSourceName->c_str(), m_logicalDevice, true, rawCode);
	if (shaderModule == VK_NULL_HANDLE)
	{
		return false;
	}

	auto pFragmentShaderRaw = std::make_shared<DrawingRawFragmentShader_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()), desc.mpName, shaderModule, rawCode, (desc.mpEntryName.get())->c_str());
	auto pFragmentShader = std::make_shared<DrawingPixelShader>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()));
	pFragmentShader->SetDesc(std::shared_ptr<DrawingResourceDesc>(desc.Clone()));
	pFragmentShader->SetResource(pFragmentShaderRaw);

	pRes = pFragmentShader;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreatePixelShaderFromFile [Implemented]" << std::endl;
#endif
	return true;
}

bool DrawingDevice_Vulkan::CreatePixelShaderFromString(const std::string& str, const DrawingPixelShaderDesc& desc, std::shared_ptr<DrawingPixelShader>& pRes)
{
	std::cout << "Vulkan CreatePixelShaderFromString" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::CreatePixelShaderFromBuffer(const void* pData, uint32_t length, const DrawingPixelShaderDesc& desc, std::shared_ptr<DrawingPixelShader>& pRes)
{
	std::cout << "Vulkan CreatePixelShaderFromBuffer" << std::endl;
	return false;
}

bool DrawingDevice_Vulkan::CreatePipelineState(const DrawingPipelineStateDesc& desc, const DrawingPipelineState::SubobjectResourceTable& subobjectResources, std::shared_ptr<DrawingPipelineState>& pRes)
{
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	std::shared_ptr<DrawingRawEffect_Vulkan> pEffectRaw = nullptr;

	// If we don't declare these structs here, they would be optimized and become corrupted in release mode
	VkPipelineInputAssemblyStateCreateInfo	inputAssembley = {};
	VkPipelineColorBlendAttachmentState		colorBlendAttachment = {};
	VkPipelineColorBlendStateCreateInfo		colorBlendCreateInfo = {};
	VkPipelineRasterizationStateCreateInfo	rasterizationState = {};
	VkPipelineDepthStencilStateCreateInfo	depthStencilState = {};
	VkViewport								viewport = {};
	VkRect2D								scissor = {};
	VkPipelineViewportStateCreateInfo		viewportState = {};
	VkPipelineMultisampleStateCreateInfo	multisampling = {};

	for (const auto& subobjectResource : subobjectResources)
	{
		switch (subobjectResource.first)
		{
		case DrawingPipelineStateDesc::ePipelineStateSubobjectType_InputLayout:
		{
			pipelineInfo.pVertexInputState = std::dynamic_pointer_cast<DrawingRawVertexFormat_Vulkan>(std::dynamic_pointer_cast<DrawingVertexFormat>(subobjectResource.second)->GetResource())->Get();
			break;
		}
		case DrawingPipelineStateDesc::ePipelineStateSubobjectType_PrimitiveTopology:
		{
			inputAssembley.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembley.topology = VK_PrimitiveTopologyEnum(std::dynamic_pointer_cast<DrawingPrimitive>(subobjectResource.second)->GetPrimitiveType());
			inputAssembley.primitiveRestartEnable = VK_FALSE;

			pipelineInfo.pInputAssemblyState = &inputAssembley;
			break;
		}
		case DrawingPipelineStateDesc::ePipelineStateSubobjectType_Effect:
		{
			pEffectRaw = std::dynamic_pointer_cast<DrawingRawEffect_Vulkan>(std::dynamic_pointer_cast<DrawingEffect>(subobjectResource.second)->GetResource());
			auto pShaderEffectRaw = std::dynamic_pointer_cast<DrawingRawShaderEffect_Vulkan>(pEffectRaw);
			assert(pShaderEffectRaw != nullptr);
			auto pVSShaderRaw = std::dynamic_pointer_cast<DrawingRawVertexShader_Vulkan>(pShaderEffectRaw->GetRawShader(DrawingRawShader::RawShader_VS));
			auto pFSShaderRaw = std::dynamic_pointer_cast<DrawingRawFragmentShader_Vulkan>(pShaderEffectRaw->GetRawShader(DrawingRawShader::RawShader_PS));
			auto pDescriptorSetLayout = pShaderEffectRaw->GetDescriptorSetLayoutObject();
			assert(pVSShaderRaw != nullptr);
			assert(pFSShaderRaw != nullptr);

			VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = pVSShaderRaw->GetStage();
			vertShaderStageInfo.module = pVSShaderRaw->GetShaderImpl()->GetModule();
			vertShaderStageInfo.pName = pVSShaderRaw->GetShaderImpl()->EntryName();
			VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
			fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage = pFSShaderRaw->GetStage();
			fragShaderStageInfo.module = pFSShaderRaw->GetShaderImpl()->GetModule();
			fragShaderStageInfo.pName = pFSShaderRaw->GetShaderImpl()->EntryName();
			shaderStages = { vertShaderStageInfo, fragShaderStageInfo };

			VkPipelineLayout pipelineLayout;
			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 1;
			pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pShaderEffectRaw->PushConstantSize());
			pipelineLayoutInfo.pPushConstantRanges = pShaderEffectRaw->GetPushConstantRanges();
			VkDescriptorSetLayout descLayout = pDescriptorSetLayout->GetDescriptorSetLayout();
			pipelineLayoutInfo.pSetLayouts = &descLayout;
			if (!VK_CheckResult(vkCreatePipelineLayout(m_logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout), "vkCreatePipelineLayout"))
			{
				return false;
			}

			pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
			pipelineInfo.pStages = shaderStages.data();
			pipelineInfo.layout = pipelineLayout;

			m_pDirectCommandManager->BindPipelineLayout(pipelineLayout);

			break;
		}
		case DrawingPipelineStateDesc::ePipelineStateSubobjectType_BlendState:
		{
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_TRUE;
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

			colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendCreateInfo.logicOpEnable = VK_FALSE;
			colorBlendCreateInfo.logicOp = VK_LOGIC_OP_COPY;
			colorBlendCreateInfo.attachmentCount = 1;
			colorBlendCreateInfo.pAttachments = &colorBlendAttachment;
			colorBlendCreateInfo.blendConstants[0] = 0.0f;
			colorBlendCreateInfo.blendConstants[1] = 0.0f;
			colorBlendCreateInfo.blendConstants[2] = 0.0f;
			colorBlendCreateInfo.blendConstants[3] = 0.0f;

			pipelineInfo.pColorBlendState = &colorBlendCreateInfo;
			break;
		}
		case DrawingPipelineStateDesc::ePipelineStateSubobjectType_RasterState:
		{
			rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationState.depthClampEnable = VK_FALSE;
			rasterizationState.rasterizerDiscardEnable = VK_FALSE; // Whether primitives are discarded immediately before the rasterization stage
			rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizationState.lineWidth = 1.0f;
			rasterizationState.cullMode = VK_CULL_MODE_NONE; //VK_CULL_MODE_BACK_BIT; // Cull back won't work for circl arc
			rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			rasterizationState.depthBiasEnable = VK_FALSE;

			pipelineInfo.pRasterizationState = &rasterizationState;
			break;
		}
		case DrawingPipelineStateDesc::ePipelineStateSubobjectType_DepthStencilState:
		{
			depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencilState.depthTestEnable = VK_TRUE;
			depthStencilState.depthWriteEnable = VK_TRUE;
			depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL; // This works for bith CAD graphs and forward renderer
			depthStencilState.depthBoundsTestEnable = VK_FALSE;
			depthStencilState.stencilTestEnable = VK_FALSE;

			pipelineInfo.pDepthStencilState = &depthStencilState;
			break;
		}
		case DrawingPipelineStateDesc::ePipelineStateSubobjectType_RenderTarget:
		{
			auto desc = std::dynamic_pointer_cast<DrawingTargetDesc>(std::dynamic_pointer_cast<DrawingTarget>(subobjectResource.second)->GetDesc());

			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(desc->mWidth);
			viewport.height = static_cast<float>(desc->mHeight);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			scissor.offset = { 0, 0 };
			scissor.extent = { desc->mWidth, desc->mHeight }; // We are using the whole viewport

			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1; // Comment: this needs to change to actual viewport number for multi-views
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_MultisampleCount(desc->mMultiSampleCount) > VK_SAMPLE_COUNT_1_BIT ? VK_TRUE : VK_FALSE;
			multisampling.rasterizationSamples = VK_MultisampleCount(desc->mMultiSampleCount);
			// TODO: this multisampling info is not complete

			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pMultisampleState = &multisampling;

			break;
		}
		}
	}

	// Alert: renderPass, subpass and basePipelineHandle info are not set by subobjectResources
	pipelineInfo.renderPass = m_renderPasses[0]; // The pipeline must only be used with an instance of any render pass compatible with the one provided
												 // See https://renderdoc.org/vkspec_chunked/chap7.html#renderpass-compatibility
												 // Our 2 render passes here are compatible, so we only specify the first render pass in create info
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	auto pPipelineState = std::make_shared<DrawingPipelineState>(shared_from_this());
	std::shared_ptr<DrawingRawPipelineState> pPipelineStateRaw = std::make_shared<DrawingRawPipelineState_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()), pEffectRaw, pipelineInfo);

	pPipelineState->SetDesc(std::shared_ptr<DrawingResourceDesc>(desc.Clone()));
	pPipelineState->SetResource(pPipelineStateRaw);

	pRes = pPipelineState;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreatePipelineState [Implemented]" << std::endl;
#endif
	return true;
}

bool DrawingDevice_Vulkan::DrawPrimitive(std::shared_ptr<DrawingPrimitive> pRes)
{
	assert(pRes != nullptr);

	if (!m_pDirectCommandManager->IsCommandBufferInUse(t_recordingCmdBufferID))
	{
		t_recordingCmdBufferID = m_pDirectCommandManager->BeginNewCommandBuffer();
	}
	if (!m_pDirectCommandManager->IsCommandBufferInRenderPass(t_recordingCmdBufferID))
	{
		VkFramebuffer framebuffer = t_pSwapchain->GetTargetFramebuffer();
		VkExtent2D extent = t_pSwapchain->GetExtent();
		if (t_awaitClearTarget)
		{
			m_pDirectCommandManager->BeginRenderPass(t_recordingCmdBufferID, m_renderPasses[0], framebuffer, extent, t_clearColor);
			t_awaitClearTarget = false;
		}
		else
		{
			m_pDirectCommandManager->BeginRenderPass(t_recordingCmdBufferID, m_renderPasses[1], framebuffer, extent);
		}
	}

	auto indexCount = pRes->GetIndexCount();
	auto instanceCount = pRes->GetInstanceCount();

	if (instanceCount == 0)
	{
		instanceCount = 1;
	}

	if (indexCount != 0)
	{
		m_pDirectCommandManager->DrawPrimitiveIndexed(t_recordingCmdBufferID, indexCount, instanceCount, pRes->GetIndexOffset(), 0, pRes->GetInstanceOffset());
	}
	else
	{
		m_pDirectCommandManager->DrawPrimitiveInstanced(t_recordingCmdBufferID, pRes->GetVertexCount(), pRes->GetInstanceCount(), pRes->GetVertexOffset(), pRes->GetInstanceOffset());
	}

	// Alert: this only works under current call sequence
	m_pDirectCommandManager->EndRenderPass(t_recordingCmdBufferID);

#if defined(SUBMIT_COMMAND_PER_DRAW_VK)
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	if (t_renderFinishedSemaphoreID == -1)
	{
		t_renderFinishedSemaphoreID = m_pSyncObjectManager->RequestSemaphoreID();
	}
#endif
#if defined(POOLED_TEXTURE_VK)
	std::vector<std::shared_ptr<SignalSempahore_Vulkan>> signalSemaphores = {};
#else
#ifndef ENABLE_TRANSFER_QUEUE_VK
	std::vector<std::shared_ptr<SignalSempahore_Vulkan>> signalSemaphores = {};
#else
	std::vector<std::shared_ptr<SignalSempahore_Vulkan>> signalSemaphores = { std::make_shared<SignalSempahore_Vulkan>(t_renderFinishedSemaphoreID, m_pSyncObjectManager->RequestSemaphore(t_renderFinishedSemaphoreID)) };
#endif
#endif

	bool relaxedSubmit = !t_newlyAcquiredImage;
	if (t_newlyAcquiredImage)
	{
		m_pDirectCommandManager->AddSemaphoreToWaitlist(std::make_shared<WaitSemaphore_Vulkan>(
			m_imageAvailableSemaphoreIDs[m_currentFrame],
			m_pSyncObjectManager->RequestSemaphore(m_imageAvailableSemaphoreIDs[m_currentFrame]),
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			true));

		t_newlyAcquiredImage = false;
	}

#if defined(CONTROLLED_TRANSFER_SUBMIT_RATE_VK)
	if (!relaxedSubmit)
	{
		m_pTransferCommandManager->SubmitCommandBuffers(VK_NULL_HANDLE, {}, false);
	} // Alert: Transfer commands must be submitted before direct commands under this setting, otherwise the semaphores won't be waited correctly
#endif

	m_pDirectCommandManager->SubmitCommandBuffers(VK_NULL_HANDLE, signalSemaphores, relaxedSubmit);
#endif

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan DrawPrimitive [Implemented]" << std::endl;
#endif
	return true;
}

bool DrawingDevice_Vulkan::Present(const std::shared_ptr<DrawingTarget> pTarget, uint32_t syncInterval)
{
	auto pSwapchainRaw = std::static_pointer_cast<DrawingRawSwapchain_Vulkan>(pTarget->GetResource());
	assert(pSwapchainRaw != nullptr);

	if (t_renderFinishedSemaphoreID == -1)
	{
		t_renderFinishedSemaphoreID = m_pSyncObjectManager->RequestSemaphoreID();
	}
	std::vector<std::shared_ptr<SignalSempahore_Vulkan>> signalSemaphores = { std::make_shared<SignalSempahore_Vulkan>(t_renderFinishedSemaphoreID, m_pSyncObjectManager->RequestSemaphore(t_renderFinishedSemaphoreID)) };

#if defined(SUBMIT_COMMAND_PER_DRAW_VK)

#if defined(CONTROLLED_TRANSFER_SUBMIT_RATE_VK)
	m_pTransferCommandManager->SubmitCommandBuffers(VK_NULL_HANDLE, {}, false);
#endif

	// TODO: find a better way to submit a fence, this is a stupid solution
	if (!m_pDirectCommandManager->IsCommandBufferInUse(t_recordingCmdBufferID))
	{
		m_pDirectCommandManager->BeginNewCommandBuffer();
	}
#if defined(POOLED_TEXTURE_VK)
	m_pDirectCommandManager->SubmitCommandBuffers(GetFrameFence(), signalSemaphores, false);
#else
#ifndef ENABLE_TRANSFER_QUEUE_VK
	m_pDirectCommandManager->SubmitCommandBuffers(GetFrameFence(), signalSemaphores, false);
#else
	m_pDirectCommandManager->SubmitCommandBuffers(GetFrameFence(), {}, false);
#endif
#endif

#else
	if (t_newlyAcquiredImage)
	{
		m_pDirectCommandManager->AddSemaphoreToWaitlist(std::make_shared<WaitSemaphore_Vulkan>(
			m_imageAvailableSemaphoreIDs[m_currentFrame],
			m_pSyncObjectManager->RequestSemaphore(m_imageAvailableSemaphoreIDs[m_currentFrame]),
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			true));

		t_newlyAcquiredImage = false;
	}
	
	m_pDirectCommandManager->SubmitCommandBuffers(GetFrameFence(), signalSemaphores, false);
#endif

	std::vector<VkFence> fences = { GetFrameFence() };
	vkWaitForFences(m_logicalDevice, static_cast<uint32_t>(fences.size()), fences.data(), VK_TRUE, std::numeric_limits<uint64_t>::max());

	pSwapchainRaw->Present(signalSemaphores, syncInterval);

	m_pSyncObjectManager->ReturnSemaphore(t_renderFinishedSemaphoreID);
	t_renderFinishedSemaphoreID = -1;

	m_pDirectCommandManager->ReleaseReservedResources();
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	m_pTransferCommandManager->ReleaseReservedResources();
#endif

	m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

#if defined(POOLED_TEXTURE_VK)
	for (auto& texPool : m_texturePools)
	{
		texPool.second->ResetPool();
		texPool.second->TransitionPoolTextures(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	}
#endif
	// TODO: find a better way to rest this index
	for (auto& effect : t_shaderEffects)
	{
		effect->ResetDescriptorUpdateIndex();
	}

	pSwapchainRaw->UpdateBackBuffer(m_pSyncObjectManager->RequestSemaphore(m_imageAvailableSemaphoreIDs[m_currentFrame]), std::numeric_limits<uint64_t>::max());
	t_newlyAcquiredImage = true;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan Present [Implemented]" << std::endl;
#endif
	return true;
}

void* DrawingDevice_Vulkan::Map(std::shared_ptr<DrawingResource> pRes, uint32_t subID, EDrawingAccessType flag, uint32_t& rowPitch, uint32_t& slicePitch, uint32_t offset, uint32_t sizeInBytes)
{
	assert(pRes != nullptr);

	if (subID < 0)
		subID = 0;

	switch (pRes->GetType())
	{
	case eResource_Texture:
		return MapResource<DrawingRawTexture_Vulkan, DrawingTexture>(pRes, subID);

	case eResource_Vertex_Buffer:
		return MapResource<DrawingRawVertexBuffer_Vulkan, DrawingVertexBuffer>(pRes, subID);

	case eResource_Index_Buffer:
		return MapResource<DrawingRawIndexBuffer_Vulkan, DrawingIndexBuffer>(pRes, subID);
	}

	return nullptr;
}

void DrawingDevice_Vulkan::UnMap(std::shared_ptr<DrawingResource> pRes, uint32_t subID)
{
	assert(pRes != nullptr);

	if (subID < 0)
		subID = 0;

	switch (pRes->GetType())
	{
	case eResource_Texture:
		return UnMapResource<DrawingRawTexture_Vulkan, DrawingTexture>(pRes, subID);

	case eResource_Vertex_Buffer:
		return UnMapResource<DrawingRawVertexBuffer_Vulkan, DrawingVertexBuffer>(pRes, subID);

	case eResource_Index_Buffer:
		return UnMapResource<DrawingRawIndexBuffer_Vulkan, DrawingIndexBuffer>(pRes, subID);
	}
}

void DrawingDevice_Vulkan::Flush()
{
	std::vector<std::shared_ptr<SignalSempahore_Vulkan>> signalSemaphores = {};
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	if (t_renderFinishedSemaphoreID != -1)
	{
		signalSemaphores.emplace_back(std::make_shared<SignalSempahore_Vulkan>(t_renderFinishedSemaphoreID, m_pSyncObjectManager->RequestSemaphore(t_renderFinishedSemaphoreID)));
	}
#endif

	if (t_newlyAcquiredImage)
	{
		m_pDirectCommandManager->AddSemaphoreToWaitlist(std::make_shared<WaitSemaphore_Vulkan>(
			m_imageAvailableSemaphoreIDs[m_currentFrame],
			m_pSyncObjectManager->RequestSemaphore(m_imageAvailableSemaphoreIDs[m_currentFrame]),
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			true));
	}

#if defined(ENABLE_TRANSFER_QUEUE_VK)
	m_pTransferCommandManager->SubmitCommandBuffers(VK_NULL_HANDLE, {}, false);
#endif

	if (m_pDirectCommandManager->SubmitCommandBuffers(GetFrameFence(), signalSemaphores, false))
	{
		t_newlyAcquiredImage = false;
	}

	// Alert: this would throttle performance
	std::vector<VkFence> fences = { GetFrameFence() };
	vkWaitForFences(m_logicalDevice, static_cast<uint32_t>(fences.size()), fences.data(), VK_TRUE, std::numeric_limits<uint64_t>::max());

	m_pDirectCommandManager->ReleaseReservedResources();
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	m_pTransferCommandManager->ReleaseReservedResources();
#endif

#if defined(POOLED_TEXTURE_VK)
	for (auto& texPool : m_texturePools)
	{
		texPool.second->ResetPool();
		texPool.second->TransitionPoolTextures(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	}
#endif
	for (auto& effect : t_shaderEffects)
	{
		effect->ResetDescriptorUpdateIndex();
	}

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan Flush [Partial]" << std::endl;
#endif
}

uint32_t DrawingDevice_Vulkan::FormatBytes(EDrawingFormatType type)
{
	return VK_FormatBytes(type);
}

bool DrawingDevice_Vulkan::CreateVertexFormat(const DrawingVertexFormatDesc& desc, std::shared_ptr<DrawingVertexFormat>& pRes)
{
	std::vector<VkVertexInputBindingDescription> bindingDescs;
	std::vector<VkVertexInputAttributeDescription> attribDescs;

	std::unordered_map<uint32_t, uint32_t> usedSlots; // (binding(slot) number, stride)
	std::unordered_map<uint32_t, VkVertexInputRate> inputRates; // binding number as key
	uint32_t usedLocation = 0;

	// Acquire the number of active bindings and calculate the stride for each of the binding
	for (const auto& elem : desc.m_inputElements)
	{
		if (usedSlots.find(elem.mSlot) == usedSlots.end())
		{
			usedSlots.emplace(elem.mSlot, VK_FormatBytes(elem.mFormat));
			inputRates.emplace(elem.mSlot, VK_VertexInputRate(elem.mInstanceStepRate));
		}
		else
		{
			usedSlots[elem.mSlot] += VK_FormatBytes(elem.mFormat);
		}
	}

	for (auto itr = usedSlots.begin(); itr != usedSlots.end(); ++itr)
	{
		VkVertexInputBindingDescription bindingDesc = {};
		bindingDesc.binding = itr->first;
		bindingDesc.stride = itr->second;
		bindingDesc.inputRate = inputRates.find(itr->first)->second;

		bindingDescs.emplace_back(bindingDesc);
	}

	for (const auto& elem : desc.m_inputElements)
	{
		VkVertexInputAttributeDescription attribDesc = {};
		attribDesc.binding = elem.mSlot;
		attribDesc.location = usedLocation;
		attribDesc.format = VK_FormatEnum(elem.mFormat);
		attribDesc.offset = elem.mOffset;

		// Alert: we assume there is no input that takes more than 4-element per vector, and we do not use component specifier to compress the space
		usedLocation += 1;

		attribDescs.emplace_back(attribDesc);
	}

	auto pVertexFormat = std::make_shared<DrawingVertexFormat>(shared_from_this());

	std::shared_ptr<DrawingRawVertexFormat> pVertexFormatRaw = std::make_shared<DrawingRawVertexFormat_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()), bindingDescs, attribDescs);
	pVertexFormat->SetDesc(std::shared_ptr<DrawingResourceDesc>(desc.Clone()));
	pVertexFormat->SetResource(pVertexFormatRaw);

	pRes = pVertexFormat;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreateVertexFormat [Implemented]" << std::endl;
#endif
	return true;
}

bool DrawingDevice_Vulkan::CreateVertexBuffer(const DrawingVertexBufferDesc& desc, std::shared_ptr<DrawingVertexBuffer>& pRes, std::shared_ptr<DrawingResource> pRefRes, const void* pData, uint32_t size)
{
	if ((pData != nullptr) && (size > desc.mSizeInBytes)) // [size] is initial size with actual data; [desc.mSizeInBytes] is the size of buffer
	{
		return false;
	}

	if (desc.mSizeInBytes == 0)
	{
#if defined(_VK_DEBUG_MSG)
		std::cout << "Vulkan: a zero-size vertex buffer will not be created." << std::endl;
#endif
		return false;
	}

	auto pVertexBuffer = std::make_shared<DrawingVertexBuffer>(shared_from_this());

	std::shared_ptr<DrawingRawVertexBuffer> pVertexBufferRaw = std::make_shared<DrawingRawVertexBuffer_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()), pData, desc.mSizeInBytes, desc.mStrideInBytes);

	pVertexBuffer->SetDesc(std::shared_ptr<DrawingResourceDesc>(desc.Clone()));
	pVertexBuffer->SetResource(pVertexBufferRaw);

	pRes = pVertexBuffer;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreateVertexBuffer [Implemented]" << std::endl;
#endif
	return true;
}

bool DrawingDevice_Vulkan::CreateIndexBuffer(const DrawingIndexBufferDesc& desc, std::shared_ptr<DrawingIndexBuffer>& pRes, std::shared_ptr<DrawingResource> pRefRes, const void* pData, uint32_t size)
{
	if ((pData != nullptr) && (size > desc.mSizeInBytes))
	{
		return false;
	}

	if (desc.mSizeInBytes == 0)
	{
#if defined(_VK_DEBUG_MSG)
		std::cout << "Vulkan: a zero-size index buffer will not be created." << std::endl;
#endif
		return true;
	}

	auto pIndexBuffer = std::make_shared<DrawingIndexBuffer>(shared_from_this());

	std::shared_ptr<DrawingRawIndexBuffer> pIndexBufferRaw = std::make_shared<DrawingRawIndexBuffer_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()), pData, desc.mSizeInBytes, desc.mStrideInBytes);

	pIndexBuffer->SetDesc(std::shared_ptr<DrawingResourceDesc>(desc.Clone()));
	pIndexBuffer->SetResource(pIndexBufferRaw);

	pRes = pIndexBuffer;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreateIndexBuffer [Implemented]" << std::endl;
#endif
	return true;
}

bool DrawingDevice_Vulkan::CreateTexture(const DrawingTextureDesc& desc, std::shared_ptr<DrawingTexture>& pRes, const void* pData[], uint32_t size[], uint32_t slices)
{
	auto pTexture = std::make_shared<DrawingTexture>(shared_from_this());
	std::shared_ptr<DrawingRawTexture> pRawTexture = nullptr;

	std::vector<VulkanSubresourceData> subResData(slices);
	uint32_t dataSize = 0;
	switch (desc.mType)
	{
	case eTexture_2D:
	case eTexture_2DArray:
	{
		RawImageCreateInfo_Vulkan createInfo;
		createInfo.format = VK_FormatEnum(desc.mFormat);
		createInfo.extent = { desc.mWidth, desc.mHeight, 1 };
		createInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		createInfo.memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;
		createInfo.mipLevels = desc.mMipLevels;

		// Alert: this is a hard coded fixer, R32G32B32_SFLOAT is incompatible with IMAGE_TILING_OPTIMAL on Quadro P4000
		if (createInfo.format == VK_FORMAT_R32G32B32_SFLOAT)
		{
			createInfo.tiling = VK_IMAGE_TILING_LINEAR;
		}

		// Alert: it may not be a good idea to create a texture pool for each created texture
#if defined(POOLED_TEXTURE_VK)
		m_texturePools.emplace(m_nextTexturePoolID, 
			std::make_shared<DrawingRawTexturePool_Vulkan>(
			std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()),
			1, 
			createInfo,
			m_nextTexturePoolID));
#endif

		if (!subResData.empty())
		{
			auto mipLevels = slices / desc.mArraySize;
			assert(mipLevels == desc.mMipLevels);
			for (uint32_t index = 0; index < desc.mArraySize; ++index)
			{
				auto bytesPerRow = desc.mBytesPerRow;
				auto bytesPerSlice = desc.mBytesPerSlice;
				for (uint32_t level = 0; level < mipLevels; ++level)
				{
					auto LOD = index * level + level;
					ZeroMemory(&subResData[LOD], sizeof(VulkanSubresourceData));
					subResData[LOD].pData = *(pData++);
					subResData[LOD].FormatBytes = VK_FormatBytes(desc.mFormat);
					subResData[LOD].RowSize = bytesPerRow;
					subResData[LOD].SliceSize = bytesPerSlice;
					subResData[LOD].Offset = dataSize;
					dataSize += size[LOD];
					//subResData[LOD].Size = dataSize - subResData[LOD].Offset;
					bytesPerRow = bytesPerRow > 1U ? bytesPerRow >> 1 : 1U;
				}
			}
#if defined(POOLED_TEXTURE_VK)
			m_texturePools[m_nextTexturePoolID]->TransitionPoolTextures(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			m_texturePools[m_nextTexturePoolID]->RequestTexture(0)->CopySubresource(subResData, dataSize, true, true); // First copy operation is completed immediately, so we don't have to request ID
			pRawTexture = m_texturePools[m_nextTexturePoolID]->RequestTexture(0);
#else
			pRawTexture = std::make_shared<DrawingRawTexture2D_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()), createInfo, subResData, dataSize);
#endif
		}

#if defined(POOLED_TEXTURE_VK)
		m_nextTexturePoolID += 1;
#endif

		break;
	}
	case eTexture_1D:
	case eTexture_1DArray:
	case eTexture_3D:
	case eTexture_Cube:
	default:
	{
		std::cout << "1D, 3D and cubic texture are not supported by Vulkan device." << std::endl;
		break;
	}
	}

	pTexture->SetDesc(std::shared_ptr<DrawingResourceDesc>(desc.Clone()));
	pTexture->SetResource(pRawTexture);

	pRes = pTexture;

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreateTexture [Implemented]" << std::endl;
#endif
	return true;
}

bool DrawingDevice_Vulkan::CreateInstance()
{
	// Check validation layers
	if (m_enableValidationLayers && !CheckValidationLayerSupport_Vulkan(m_validationLayers))
	{
		throw std::runtime_error("Validation layers requested, but not available.");
	}

	// Check extentions support
	if (!CheckAvailableInstanceExtensions_Vulkan(m_availableExtensions))
	{
		return false;
	}
	for (auto& extension : m_desiredExtensions)
	{
		if (!IsExtensionSupported_Vulkan(m_availableExtensions, extension))
		{
			std::cout << "Extension named '" << extension << "' is not supported." << std::endl;
			return false;
		}
	}

	// Generate application info
	m_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	m_appInfo.pApplicationName = "Graphics Engine";
	m_appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	m_appInfo.pEngineName = "Eternal Vulkan";
	m_appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	m_appInfo.apiVersion = VK_API_VERSION_1_1;

	// Generate creation info
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &m_appInfo;
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_desiredExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = m_desiredExtensions.size() > 0 ? m_desiredExtensions.data() : nullptr;

	// Configure debug messenger
	if (m_enableValidationLayers)
	{
		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = m_validationLayers.data();
		instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)& GetDebugUtilsMessengerCreateInfo_Vulkan();
	}
	else
	{
		instanceCreateInfo.enabledLayerCount = 0;
		instanceCreateInfo.pNext = nullptr;
	}

	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);
	if ((result != VK_SUCCESS) || m_instance == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Could not create Vulkan instance.");
		return false;
	}

	return true;
}

bool DrawingDevice_Vulkan::SetupDebugMessenger()
{
	if (!m_enableValidationLayers)
	{
		return true;
	}

	VkResult result = CreateDebugUtilsMessengerEXT_Vulkan(m_instance, nullptr, &m_debugMessenger);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to setup debug messenger");
		return false;
	}

	return true;
}

bool DrawingDevice_Vulkan::CreatePresentationSurface()
{
#if defined(VK_USE_PLATFORM_WIN32_KHR)

	WindowParameters_Vulkan windowParameters;
	windowParameters.HWnd = reinterpret_cast<HWND>(gpGlobal->GetConfiguration<AppConfiguration>().GetAppHandle());
	windowParameters.HInstance = GetModuleHandle(nullptr);

	VkWin32SurfaceCreateInfoKHR surfaceCreationInfo;
	surfaceCreationInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreationInfo.hinstance = windowParameters.HInstance;
	surfaceCreationInfo.hwnd = windowParameters.HWnd;
	surfaceCreationInfo.pNext = nullptr;
	surfaceCreationInfo.flags = 0;

	VkResult result = vkCreateWin32SurfaceKHR(m_instance, &surfaceCreationInfo, nullptr, &m_surface);

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	//...

#elif defined(VK_USE_PLATFORM_XCB_KHR)
	//...

#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
	//...

#elif defined(VK_USE_PLATFORM_IOS_MVK)
	//...

#elif defined(VK_USE_PLATFORM_MACOS_MVK)
	//...

#endif

	if ((result != VK_SUCCESS) || m_surface == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Could not create presentation surface.");
		return false;
	}

	return true;
}

bool DrawingDevice_Vulkan::IsDeviceSuitable(const VkPhysicalDevice & device)
{
	QueueFamilyIndices_Vulkan indices = FindQueueFamilies_Vulkan(device, m_surface);

	bool extensionsSupported = CheckDeviceExtensionsSupport_Vulkan(device, m_deviceExtensions);

	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapchainSupportDetails_Vulkan swapChainSupport = QuerySwapchainSupport_Vulkan(device, m_surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	// Could add customized selection criteria here

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool DrawingDevice_Vulkan::SelectPhysicalDevice()
{
	uint32_t deviceCount = 0;
	VkResult result;

	result = vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	if (deviceCount == 0 || (result != VK_SUCCESS))
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support.");
		return false;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			m_physicalDevice = device;
			break;
		}
	}

	if (m_physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a suitable GPU.");
		return false;
	}

	vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);
#if defined(_DEBUG)
	VK_PrintPhysicalDeviceInfo(m_deviceProperties);
#endif

	return true;
}

bool DrawingDevice_Vulkan::CreateLogicalDevice()
{
	m_queueFamilyIndices = FindQueueFamilies_Vulkan(m_physicalDevice, m_surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	std::set<uint32_t> uniqueQueueFamilies = { m_queueFamilyIndices.graphicsFamily.value(), m_queueFamilyIndices.presentFamily.value(), m_queueFamilyIndices.transferFamily.value() };
#else
	std::set<uint32_t> uniqueQueueFamilies = { m_queueFamilyIndices.graphicsFamily.value(), m_queueFamilyIndices.presentFamily.value() };
#endif

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = &m_deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

	if (m_enableValidationLayers)
	{
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = m_validationLayers.data();
	}
	else
	{
		deviceCreateInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_logicalDevice);

	if (result != VK_SUCCESS || m_logicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to create logical device.");
		return false;
	}

	vkGetDeviceQueue(m_logicalDevice, m_queueFamilyIndices.graphicsFamily.value(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_logicalDevice, m_queueFamilyIndices.presentFamily.value(), 0, &m_presentQueue);
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	vkGetDeviceQueue(m_logicalDevice, m_queueFamilyIndices.transferFamily.value(), 0, &m_transferQueue);
#endif

	return true;
}

bool DrawingDevice_Vulkan::SetupCommandManager()
{
	m_pDirectCommandManager = std::make_shared<DrawingCommandManager_Vulkan>(
		std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()),
		m_graphicsQueue,
		eCommandList_Direct
		);

	m_pDirectCommandManager->CreateCommandPool(m_queueFamilyIndices.graphicsFamily.value());
	m_pDirectCommandManager->AllocateCommandBuffers(2);

#if defined(ENABLE_TRANSFER_QUEUE_VK)
	m_pTransferCommandManager = std::make_shared<DrawingCommandManager_Vulkan>(
		std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()),
		m_transferQueue,
		eCommandList_Copy
		);

	m_pTransferCommandManager->CreateCommandPool(m_queueFamilyIndices.transferFamily.value());
	m_pTransferCommandManager->AllocateCommandBuffers(2);
#endif

	m_pSyncObjectManager = std::make_shared<DrawingCommandSyncObejctManager_Vulkan>(
		std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()),
		MAX_FRAMES_IN_FLIGHT,
		MAX_FRAMES_IN_FLIGHT
		);

	return true;
}

bool DrawingDevice_Vulkan::SetupUploadAllocator()
{
	m_pUploadAllocator = std::make_shared<DrawingUploadAllocator_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()));
	return true;
}

bool DrawingDevice_Vulkan::SetupDescriptorAllocator()
{
	m_pDescriptorAllocator = std::make_shared<DrawingDescriptorAllocator_Vulkan>(std::static_pointer_cast<DrawingDevice_Vulkan>(shared_from_this()));
	return true;
}

VkSurfaceFormatKHR DrawingDevice_Vulkan::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> & availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return { VK_FORMAT_UNDEFINED , VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }; // Alert: this could be incorrect
}

VkPresentModeKHR DrawingDevice_Vulkan::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> & availableModes)
{
	// For currrent implementation, we have 4 level of present modes
	for (int lv = 0; lv < 4; ++lv)
	{
		for (size_t i = 0; i < availableModes.size(); ++i)
		{
			if (availableModes[i] == m_optimalPresentModes[lv])
			{
#if defined(_DEBUG)
				std::string mode = "";
				switch (availableModes[i])
				{
				case VK_PRESENT_MODE_FIFO_KHR:
					mode = "FIFO";
					break;
				case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
					mode = "FIFO_RELAXED";
					break;
				case VK_PRESENT_MODE_MAILBOX_KHR:
					mode = "MAILBOX";
					break;
				case VK_PRESENT_MODE_IMMEDIATE_KHR:
					mode = "IMMEDIATE";
					break;
				}
				std::cout << "Present Mode: " << mode << std::endl;
#endif
				return availableModes[i];
			}
		}
	}

	// Fallback
	throw std::runtime_error("No compatible present mode found.");
	return m_optimalPresentModes[3];
}

VkExtent2D DrawingDevice_Vulkan::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		VkExtent2D actualExtent =
		{
			gpGlobal->GetConfiguration<AppConfiguration>().GetWidth(),
			gpGlobal->GetConfiguration<AppConfiguration>().GetHeight()
		};

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(actualExtent.width, capabilities.maxImageExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(actualExtent.height, capabilities.maxImageExtent.height));

		return actualExtent;
	}
}

VkImageView DrawingDevice_Vulkan::CreateImageView(VkImage& image, VkImageViewType type, VkImageAspectFlags aspect, VkFormat format, uint32_t mipLevels)
{
	// Alert: some parameters are hard coded
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = type;
	createInfo.format = format;
	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.subresourceRange.aspectMask = aspect;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = mipLevels;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (!VK_CheckResult(vkCreateImageView(m_logicalDevice, &createInfo, nullptr, &imageView), "vkCreateImageView"))
	{
		return VK_NULL_HANDLE;
	}

	return imageView;
}

bool DrawingDevice_Vulkan::PrepareRenderPasses()
{
	assert(m_renderPasses.size() == 0);

	// First render pass per frame (including clear operation and layout transition)

	VkAttachmentDescription depthAttachmentDesc = {};
	depthAttachmentDesc.format =		FindDepthFormat();
	depthAttachmentDesc.samples =		VK_SAMPLE_COUNT_1_BIT; // An image with one sample per pixel
	depthAttachmentDesc.loadOp =		VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachmentDesc.storeOp =		VK_ATTACHMENT_STORE_OP_STORE; // This could be redundant if there is only one pass
	depthAttachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // We are not using stencil right now
	depthAttachmentDesc.stencilStoreOp =VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachmentDesc.finalLayout =	VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription colorAttachmentDesc = {};
	colorAttachmentDesc.format =		t_pSwapchain->GetTargetFormat();
	colorAttachmentDesc.samples =		VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentDesc.loadOp =		VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachmentDesc.storeOp =		VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentDesc.stencilStoreOp =VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentDesc.finalLayout =	VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	std::vector<VkAttachmentDescription> attachments = { colorAttachmentDesc, depthAttachmentDesc };

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDesc = {};
	subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDesc.colorAttachmentCount = 1; // TODO: this needs to be changed according to actual attachment count
	subpassDesc.pColorAttachments = &colorAttachmentRef;
	subpassDesc.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency subpassDependency = {};
	subpassDependency.srcSubpass =		VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass =		0;
	subpassDependency.srcStageMask =	VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask =	0;
	subpassDependency.dstStageMask =	VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.dstAccessMask =	VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::vector<VkSubpassDescription> subpassDescs = { subpassDesc };
	std::vector<VkSubpassDependency>  dependencies = { subpassDependency };

	m_renderPasses.emplace_back(CreateRenderPass(attachments, subpassDescs, dependencies));

	// Subsequent render pass per frame (no clear operation and layout transition)

	depthAttachmentDesc = {};
	depthAttachmentDesc.format =		FindDepthFormat();
	depthAttachmentDesc.samples =		VK_SAMPLE_COUNT_1_BIT;
	depthAttachmentDesc.loadOp =		VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachmentDesc.storeOp =		VK_ATTACHMENT_STORE_OP_STORE; // TODO: the last render pass in a frame doesn't need to store depth values, this could still be redundant
	depthAttachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachmentDesc.stencilStoreOp =VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachmentDesc.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	depthAttachmentDesc.finalLayout =	VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	colorAttachmentDesc = {};
	colorAttachmentDesc.format =		t_pSwapchain->GetTargetFormat();
	colorAttachmentDesc.samples =		VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentDesc.loadOp =		VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentDesc.storeOp =		VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentDesc.stencilStoreOp =VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentDesc.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	colorAttachmentDesc.finalLayout =	VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	attachments.clear();
	attachments = { colorAttachmentDesc, depthAttachmentDesc };

	colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	subpassDesc = {};
	subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDesc.colorAttachmentCount = 1;
	subpassDesc.pColorAttachments = &colorAttachmentRef;
	subpassDesc.pDepthStencilAttachment = &depthAttachmentRef;

	subpassDependency = {};
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask = 0;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	subpassDescs.clear();
	subpassDescs = { subpassDesc };
	dependencies.clear();
	dependencies = { subpassDependency };

	m_renderPasses.emplace_back(CreateRenderPass(attachments, subpassDescs, dependencies));

	return true;
}

VkRenderPass DrawingDevice_Vulkan::CreateRenderPass(const std::vector<VkAttachmentDescription>& attachmentDescs, const std::vector<VkSubpassDescription>& subpassDescs, const std::vector<VkSubpassDependency>& dependencies)
{
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentDescs.size());
	renderPassInfo.pAttachments = attachmentDescs.data();
	renderPassInfo.subpassCount = static_cast<uint32_t>(subpassDescs.size());
	renderPassInfo.pSubpasses = subpassDescs.data();
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

#if defined(_VK_DEBUG_MSG)
	std::cout << "Vulkan CreateRenderPass [Implemented]" << std::endl;
#endif

	VkRenderPass renderPassHandle;
	VK_CheckResult(vkCreateRenderPass(m_logicalDevice, &renderPassInfo, nullptr, &renderPassHandle), "vkCreateRenderPass");

	return renderPassHandle;
}

// TODO: merge this function with engine implementation
VkSampler DrawingDevice_Vulkan::CreateTextureSampler()
{
	VkSampler sampler = VK_NULL_HANDLE;

	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

	// TODO: these parameters should be customized
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_FALSE; // AF feature is not enabled, so we set this to false right now 
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod = 0;
	samplerInfo.maxLod = static_cast<float>(1);
	samplerInfo.mipLodBias = 0;

	VK_CheckResult(vkCreateSampler(m_logicalDevice, &samplerInfo, nullptr, &sampler), "vkCreateSampler");

	return sampler;
}

bool DrawingDevice_Vulkan::GenerateMipmaps(DrawingRawImage_Vulkan& image)
{
	if (!IsLinearBlittingSupported_Vulkan(m_physicalDevice, image.m_format))
	{
		throw std::runtime_error("Texture image format does not support linear blitting.");
		return false;
	}

	DrawingCommandBuffer_Vulkan commandBuffer = m_pDirectCommandManager->BeginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image.m_image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = image.m_extent.width;
	int32_t mipHeight = image.m_extent.height;

	for (uint32_t i = 1; i < image.m_mipLevels; ++i)
	{
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer.m_bufferHandle,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		VkImageBlit blit = {};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer.m_bufferHandle,
			image.m_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			image.m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer.m_bufferHandle,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		if (mipWidth > 1)
		{
			mipWidth /= 2;
		}
		if (mipHeight > 1)
		{
			mipHeight /= 2;
		}
	}

	barrier.subresourceRange.baseMipLevel = image.m_mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer.m_bufferHandle,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	return m_pDirectCommandManager->EndSingleTimeCommands(commandBuffer);
}

bool DrawingDevice_Vulkan::RecreateSwapchain()
{
	vkDeviceWaitIdle(m_logicalDevice);

	CleanUpSwapchain();

	// Create swapchain

	// Create render pass

	// Create graphics pipeline

	// Create frame buffers

	// Create uniform buffers

	// Create descriptor pools & sets

	// (Re-create command buffers if needed)

	return false;
}

void DrawingDevice_Vulkan::CleanUpSwapchain()
{
	// Destroy framebuffers

	// Destroy graphics pipeline

	// Destroy graphics pipeline layout

	// Destroy render passes

	// (Free command buffers if needed)

	// Destroy current swapchain

	// Free uniform buffers

	m_pDescriptorAllocator->DestroyPools();
}

VkFormat DrawingDevice_Vulkan::FindDepthFormat()
{
	return FindSupportedFormat_Vulkan(
		m_physicalDevice,
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

std::shared_ptr<DrawingCommandManager_Vulkan> DrawingDevice_Vulkan::GetCommandManager(EDrawingCommandListType type) const
{
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	switch (type)
	{
	case eCommandList_Direct:
		return m_pDirectCommandManager;
	case eCommandList_Copy:
		return m_pTransferCommandManager;
	default:
		throw std::runtime_error("Unsupported command manager type.");
		break;
	}
#else
	return m_pDirectCommandManager;
#endif
}

std::shared_ptr<DrawingCommandSyncObejctManager_Vulkan> DrawingDevice_Vulkan::GetSyncObjectManager() const
{
	return m_pSyncObjectManager;
}

std::shared_ptr<DrawingUploadAllocator_Vulkan> DrawingDevice_Vulkan::GetUploadAllocator() const
{
	return m_pUploadAllocator;
}

std::shared_ptr<DrawingDescriptorAllocator_Vulkan> DrawingDevice_Vulkan::GetDescriptorAllocator() const
{
	return m_pDescriptorAllocator;
}

#if defined(POOLED_TEXTURE_VK)
std::shared_ptr<DrawingRawTexturePool_Vulkan> DrawingDevice_Vulkan::GetTexturePool(uint32_t id)
{
	assert(m_texturePools.find(id) != m_texturePools.end());
	return m_texturePools[id];
}
#endif

#if defined(POOLED_STAGING_BUFFER_VK)
std::shared_ptr<DrawingRawStagingBufferPool_Vulkan> DrawingDevice_Vulkan::GetStagingBufferPool() const
{
	return m_pStagingBufferPool;
}
#endif

const VkPhysicalDeviceLimits& DrawingDevice_Vulkan::GetDeviceLimits() const
{
	return m_deviceProperties.limits;
}

const VkFence DrawingDevice_Vulkan::GetFrameFence() const
{
	assert(m_currentFrame < m_fenceIDs.size());
	return m_pSyncObjectManager->RequestFence(m_fenceIDs[m_currentFrame]);
}

template<typename T, typename U>
void* DrawingDevice_Vulkan::MapResource(std::shared_ptr<DrawingResource> pRes, uint32_t subID)
{
	auto pResWrap = std::dynamic_pointer_cast<U>(pRes);
	assert(pResWrap != nullptr);

	auto pMapRes = std::dynamic_pointer_cast<T>(pResWrap->GetResource());
	assert(pMapRes != nullptr);

	return pMapRes->MapResource();
}

template<typename T, typename U>
void DrawingDevice_Vulkan::UnMapResource(std::shared_ptr<DrawingResource> pRes, uint32_t aSubID)
{
	auto pResWrap = std::dynamic_pointer_cast<U>(pRes);
	assert(pResWrap != nullptr);

	auto pMapRes = std::dynamic_pointer_cast<T>(pResWrap->GetResource());
	assert(pMapRes != nullptr);

	pMapRes->UnmapResource();
}