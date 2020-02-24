#include "DrawingCommandManager_Vulkan.h"
#include "DrawingDevice_Vulkan.h"
#include "DrawingUtil_Vulkan.h"
#include "DrawingRawResource_Vulkan.h"
#include "DrawingExtensionWrangler_Vulkan.h"
#include "Matrix.h"
#include "Vector.h"
#include <array>


using namespace Engine;

DrawingCommandManager_Vulkan::DrawingCommandManager_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const VkQueue queue, EDrawingCommandListType type)
	: m_pDevice(pDevice), m_queue(queue), m_type(type), m_pipelineLayout(VK_NULL_HANDLE)
{
	m_isValid = true;
	m_commandPool = VK_NULL_HANDLE;
	m_singleTimeCommandPool = VK_NULL_HANDLE;
}

DrawingCommandManager_Vulkan::~DrawingCommandManager_Vulkan()
{
	ReleaseReservedResources();
	Destroy();
}

void DrawingCommandManager_Vulkan::Destroy()
{
	if (m_isValid)
	{
		FreeBuffers();

		vkDestroyCommandPool(m_pDevice->m_logicalDevice, m_commandPool, nullptr);
		vkDestroyCommandPool(m_pDevice->m_logicalDevice, m_singleTimeCommandPool, nullptr);

		m_isValid = false;
	}
}

void DrawingCommandManager_Vulkan::FreeBuffers()
{
	if (m_isValid)
	{
		std::vector<VkCommandBuffer> commandBuffers;
		for (auto& cmdBuffer : m_commandBuffers)
		{
			commandBuffers.emplace_back(cmdBuffer.m_bufferHandle);
		}
		vkFreeCommandBuffers(m_pDevice->m_logicalDevice, m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), commandBuffers.data());

		commandBuffers.clear();
		for (auto& cmdBuffer : m_singleTimeCommandBuffers)
		{
			commandBuffers.emplace_back(cmdBuffer.m_bufferHandle);
		}
		vkFreeCommandBuffers(m_pDevice->m_logicalDevice, m_singleTimeCommandPool, static_cast<uint32_t>(m_singleTimeCommandBuffers.size()), commandBuffers.data());

		m_commandBuffers.clear();
		m_singleTimeCommandBuffers.clear();
	}
}

EDrawingCommandListType DrawingCommandManager_Vulkan::GetCommandBufferType() const
{
	return m_type;
}

bool DrawingCommandManager_Vulkan::CreateCommandPool(uint32_t queueFamilyIndex)
{
	assert(m_commandPool == VK_NULL_HANDLE);
	assert(m_singleTimeCommandPool == VK_NULL_HANDLE);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndex;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // TODO: figure out the performance overhead of this flag

	if (!VK_CheckResult(vkCreateCommandPool(m_pDevice->m_logicalDevice, &poolInfo, nullptr, &m_commandPool), "vkCreateCommandPool"))
	{
		return false;
	}

	if (!VK_CheckResult(vkCreateCommandPool(m_pDevice->m_logicalDevice, &poolInfo, nullptr, &m_singleTimeCommandPool), "vkCreateCommandPool"))
	{
		return false;
	}

	return true;
}

bool DrawingCommandManager_Vulkan::AllocateCommandBuffers(uint32_t size)
{
	assert(m_commandBuffers.size() <= MAX_COMMAND_BUFFER);

	std::vector<VkCommandBuffer> cmdBufferHandles(size);

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(size);

	if (!VK_CheckResult(vkAllocateCommandBuffers(m_pDevice->m_logicalDevice, &allocInfo, cmdBufferHandles.data()), "vkAllocateCommandBuffers"))
	{
		return false;
	}

	for (auto& cmdBuffer : cmdBufferHandles)
	{
		m_commandBuffers.emplace_back(cmdBuffer);
	}

	return true;
}

uint32_t DrawingCommandManager_Vulkan::BeginNewCommandBuffer()
{
	assert(m_commandBuffers.size() > 0);

	uint32_t id = -1;
	bool foundAvailable = false;
	for (size_t i = 0; i < m_commandBuffers.size(); ++i)
	{
		if (!m_commandBuffers[i].m_inUse && !m_commandBuffers[i].m_inExecution)
		{
			id = static_cast<uint32_t>(i);
			foundAvailable = true;
			break;
		}
	}
	if (!foundAvailable)
	{
		id = static_cast<uint32_t>(m_commandBuffers.size());
		if (!AllocateCommandBuffers(1)) // We can allocate more at once if we want to reduce the number of allocations
		{
			throw std::runtime_error("Failed to allocate new command buffer.");
			return -1;
		}
	}

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // We only submit the command buffer once

	if (!VK_CheckResult(vkBeginCommandBuffer(m_commandBuffers[id].m_bufferHandle, &beginInfo), "vkBeginCommandBuffer"))
	{
		return false;
	}

	m_commandBuffers[id].m_inUse = true;
	m_commandBuffers[id].m_inRenderPass = false;

	return id;
}

bool DrawingCommandManager_Vulkan::BindVertexBuffer(uint32_t id, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* vertexBuffer, const VkDeviceSize* offsets)
{
	assert(id < m_commandBuffers.size());
	assert(m_commandBuffers[id].m_inUse);
	vkCmdBindVertexBuffers(m_commandBuffers[id].m_bufferHandle, firstBinding, bindingCount, vertexBuffer, offsets);
	return true;
}

bool DrawingCommandManager_Vulkan::BindIndexBuffer(uint32_t id, const VkBuffer indexBuffer, const VkDeviceSize offset, VkIndexType type)
{
	assert(id < m_commandBuffers.size());
	assert(m_commandBuffers[id].m_inUse);
	vkCmdBindIndexBuffer(m_commandBuffers[id].m_bufferHandle, indexBuffer, offset, type);
	return true;
}

bool DrawingCommandManager_Vulkan::BeginRenderPass(uint32_t id, const VkRenderPass renderPass, const VkFramebuffer frameBuffer, const VkExtent2D& swapchainExtent, const float4& clearColor)
{
	assert(id < m_commandBuffers.size());
	assert(m_commandBuffers[id].m_inUse);

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = frameBuffer;
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapchainExtent;

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size()); // clearValueCount must be greater than the largest attachment index in renderPass that specifies a loadOp (or stencilLoadOp, if the attachment has a depth/stencil format) of VK_ATTACHMENT_LOAD_OP_CLEAR
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(m_commandBuffers[id].m_bufferHandle, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	m_commandBuffers[id].m_inRenderPass = true;

	return true;
}

void DrawingCommandManager_Vulkan::BindPipelineLayout(const VkPipelineLayout pipelineLayout)
{
	m_pipelineLayout = pipelineLayout;
}

void DrawingCommandManager_Vulkan::AddSemaphoreToWaitlist(const std::shared_ptr<WaitSemaphore_Vulkan> pWaitSemaphore)
{
	m_pWaitSemaphores.emplace_back(pWaitSemaphore);
}

bool DrawingCommandManager_Vulkan::BindPipeline(uint32_t id, VkPipelineBindPoint bindPoint, VkPipeline pipeline)
{
	assert(id < m_commandBuffers.size());
	assert(m_commandBuffers[id].m_inUse);
	vkCmdBindPipeline(m_commandBuffers[id].m_bufferHandle, bindPoint, pipeline);
	return true;
}

bool DrawingCommandManager_Vulkan::UpdatePushConstant(uint32_t id, VkShaderStageFlags shaderStage, uint32_t size, const void* pData, uint32_t offset)
{
	assert(id < m_commandBuffers.size());
	assert(m_commandBuffers[id].m_inUse);
	vkCmdPushConstants(m_commandBuffers[id].m_bufferHandle, m_pipelineLayout, shaderStage, offset, size, pData);
	return true;
}

bool DrawingCommandManager_Vulkan::BindDescriptorSets(uint32_t id, const VkPipelineBindPoint bindPoint, const std::vector<VkDescriptorSet> descriptorSets, uint32_t firstSet)
{
	assert(id < m_commandBuffers.size());
	assert(m_commandBuffers[id].m_inUse);
	vkCmdBindDescriptorSets(m_commandBuffers[id].m_bufferHandle, bindPoint, m_pipelineLayout, firstSet, static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr);
	return true;
}

bool DrawingCommandManager_Vulkan::DrawPrimitiveIndexed(uint32_t id, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
{
	assert(id < m_commandBuffers.size());
	assert(m_commandBuffers[id].m_inRenderPass);
	if (indexCount > 0 && instanceCount > 0)
	{
		vkCmdDrawIndexed(m_commandBuffers[id].m_bufferHandle, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	return true;
}

bool DrawingCommandManager_Vulkan::DrawPrimitiveInstanced(uint32_t id, uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset)
{
	assert(id < m_commandBuffers.size());
	assert(m_commandBuffers[id].m_inRenderPass);
	if (vertexCount > 0 && instanceCount > 0)
	{
		vkCmdDraw(m_commandBuffers[id].m_bufferHandle, vertexCount, instanceCount, vertexOffset, instanceOffset);
	}

	return true;
}

bool DrawingCommandManager_Vulkan::EndRenderPass(uint32_t id)
{
	assert(id < m_commandBuffers.size());
	assert(m_commandBuffers[id].m_inUse);
	assert(m_commandBuffers[id].m_inRenderPass);

	vkCmdEndRenderPass(m_commandBuffers[id].m_bufferHandle);

	m_commandBuffers[id].m_inRenderPass = false;

	return true;
}

bool DrawingCommandManager_Vulkan::SubmitCommandBuffers(const VkFence fence, const std::vector<std::shared_ptr<SignalSempahore_Vulkan>>& signalSemaphores, bool relaxed)
{
	// TODO: This part is too ugly, needs clean up

#if defined(CONTROLLED_SUBMIT_RATE_VK)
	for (auto& semaphore : signalSemaphores)
	{
		m_signalSemaphores.emplace_back(semaphore);
	}
	uint32_t submitLimit = CONTROLLED_SUBMIT_RATE_VK;
#if defined(ENABLE_TRANSFER_QUEUE_VK)
#ifndef POOLED_TEXTURE_VK
	throw std::runtime_error("CONTROLLED_SUBMIT_RATE_VK requires POOLED_TEXTURE_VK when ENABLE_TRANSFER_QUEUE_VK is present.");
#endif
#if defined(CONTROLLED_TRANSFER_SUBMIT_RATE_VK)
	submitLimit = m_type == eCommandList_Copy ? CONTROLLED_TRANSFER_SUBMIT_RATE_VK : CONTROLLED_SUBMIT_RATE_VK;
#else
	relaxed = m_type == eCommandList_Copy ? false : relaxed;
#endif
#endif
	if (relaxed)
	{
#ifndef COMPACT_COMMAND_BUFFER_VK
		uint32_t counter = 0;
		for (auto& cmdBuffer : m_commandBuffers)
		{
			if (cmdBuffer.m_inUse)
			{
				if (cmdBuffer.m_inRenderPass)
				{
					vkCmdEndRenderPass(cmdBuffer.m_bufferHandle);
					cmdBuffer.m_inRenderPass = false;
				}
				VK_CheckResult(vkEndCommandBuffer(cmdBuffer.m_bufferHandle), "vkEndCommandBuffer");
				cmdBuffer.m_inUse = false;
				cmdBuffer.m_inExecution = true; // This is not accurate, but since no one is inquiring execution state, it won't cause trouble for now
				cmdBuffer.m_awaitSubmit = true;
			}
			if (cmdBuffer.m_awaitSubmit)
			{
				counter++;
			}
		}
		if (counter < submitLimit)
		{
			return false;
		}
#else
		m_stackedSubmissionTimes += 1;
		if (m_stackedSubmissionTimes < submitLimit)
		{
			return false;
		}
		else
		{
			m_stackedSubmissionTimes = 0;
		}
#endif
	}
#endif

	// This should work since we won't have too many command buffers
	std::vector<VkCommandBuffer> buffersAwaitSubmit;
	for (auto& cmdBuffer : m_commandBuffers)
	{
		if (cmdBuffer.m_inUse)
		{
			if (cmdBuffer.m_inRenderPass)
			{
				vkCmdEndRenderPass(cmdBuffer.m_bufferHandle);
				cmdBuffer.m_inRenderPass = false;
			}
			VK_CheckResult(vkEndCommandBuffer(cmdBuffer.m_bufferHandle), "vkEndCommandBuffer");
			cmdBuffer.m_inUse = false;
			cmdBuffer.m_inExecution = true;
			buffersAwaitSubmit.emplace_back(cmdBuffer.m_bufferHandle);
		}
#if defined(CONTROLLED_SUBMIT_RATE_VK)
		else if (cmdBuffer.m_awaitSubmit)
		{
			buffersAwaitSubmit.emplace_back(cmdBuffer.m_bufferHandle);
			cmdBuffer.m_awaitSubmit = false;
		}
#endif
	}
	if (buffersAwaitSubmit.size() == 0)
	{
#if defined(_VK_DEBUG_MSG)
		std::cout << "Vulkan: No command buffer needs to be submitted." << std::endl;
#endif
		return false;
	}

#if defined(CONTROLLED_SUBMIT_RATE_VK)
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	// TODO: this is a quick but nasty solution, needs better idea
	if (m_type == eCommandList_Copy)
	{
		t_requiresCopyWait = true;
	}
	else
	{
		auto pTransferCommandManager = m_pDevice->GetCommandManager(eCommandList_Copy);
		auto pSyncObjectManager = m_pDevice->GetSyncObjectManager();

		// Wait until previous copy operations are finished
		if (pTransferCommandManager->t_requiresCopyWait)
		{
			if (!pTransferCommandManager->IsCommandBufferInUse(m_pDevice->t_recordingTransCmdBufferID))
			{
				pTransferCommandManager->BeginNewCommandBuffer();
			}
			uint32_t copyFinishedSemaphoreID = pSyncObjectManager->RequestSemaphoreID();
			m_pWaitSemaphores.emplace_back(std::make_shared<WaitSemaphore_Vulkan>(copyFinishedSemaphoreID, pSyncObjectManager->RequestSemaphore(copyFinishedSemaphoreID), VK_PIPELINE_STAGE_TRANSFER_BIT, false, eWaitType_Copy));
			std::vector<std::shared_ptr<SignalSempahore_Vulkan>> copyFinishedSemaphore = { std::make_shared<SignalSempahore_Vulkan>(copyFinishedSemaphoreID, pSyncObjectManager->RequestSemaphore(copyFinishedSemaphoreID), eWaitType_Copy) };
			pTransferCommandManager->SubmitCommandBuffers(VK_NULL_HANDLE, copyFinishedSemaphore, false);
			pTransferCommandManager->t_requiresCopyWait = false;
		}
	}
#endif
#endif

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	submitInfo.commandBufferCount = static_cast<uint32_t>(buffersAwaitSubmit.size());
	submitInfo.pCommandBuffers = buffersAwaitSubmit.data();

	std::vector<VkSemaphore> waitSemaphores;
	std::vector<VkPipelineStageFlags> waitStages;

	if (m_pWaitSemaphores.size() > 0 && static_cast<size_t>(m_waitSemaphoreHead) < m_pWaitSemaphores.size())
	{
		for (size_t i = m_waitSemaphoreHead; i < m_pWaitSemaphores.size(); ++i)
		{
			waitSemaphores.emplace_back(m_pWaitSemaphores[i]->semaphore);
			waitStages.emplace_back(m_pWaitSemaphores[i]->waitStage);
			m_waitSemaphoreHead++;
		}

		submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
		submitInfo.pWaitSemaphores = waitSemaphores.data();
		submitInfo.pWaitDstStageMask = waitStages.data();
	}
	else
	{
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitDstStageMask = 0;
	}

	std::vector<VkSemaphore> semaphoresToSignal;

#if defined(CONTROLLED_SUBMIT_RATE_VK)
	if (m_signalSemaphores.size() > 0)
	{
		for (auto& semaphore : m_signalSemaphores)
		{
			semaphoresToSignal.emplace_back(semaphore->semaphore);
		}
		submitInfo.signalSemaphoreCount = static_cast<uint32_t>(semaphoresToSignal.size());
		submitInfo.pSignalSemaphores = semaphoresToSignal.data();
	}
#else
	if (signalSemaphores.size() > 0)
	{
		for (auto& semaphore : signalSemaphores)
		{
			semaphoresToSignal.emplace_back(semaphore->semaphore);
		}
		submitInfo.signalSemaphoreCount = static_cast<uint32_t>(semaphoresToSignal.size());
		submitInfo.pSignalSemaphores = semaphoresToSignal.data();
	}
#endif
	else
	{
		submitInfo.signalSemaphoreCount = 0;
	}
	
	if (fence != VK_NULL_HANDLE)
	{
		vkResetFences(m_pDevice->m_logicalDevice, 1, &fence);
	}

	if (!VK_CheckResult(vkQueueSubmit(m_queue, 1, &submitInfo, fence), "vkQueueSubmit"))
	{
		return false;
	}

	buffersAwaitSubmit.clear();
	waitSemaphores.clear();
	waitStages.clear();
	semaphoresToSignal.clear();
#if defined(CONTROLLED_SUBMIT_RATE_VK)
	m_signalSemaphores.clear();
#if defined(COMPACT_COMMAND_BUFFER_VK)
	m_stackedSubmissionTimes = 0;
#endif
#endif

	return true;
}

bool DrawingCommandManager_Vulkan::ReleaseReservedResources()
{
	for (auto& cmdBuffer : m_commandBuffers)
	{
		cmdBuffer.m_inExecution = false;
	}

	for (auto& preservedResource : m_pStagingBuffers)
	{
		preservedResource->Free();
	}
	m_pStagingBuffers.clear();

#if defined(POOLED_STAGING_BUFFER_VK)
	auto pStagingBufferPool = m_pDevice->GetStagingBufferPool();
	for (auto& id : m_poolStagingBufferIDs)
	{
		pStagingBufferPool->ReturnStagingBuffer(id);
	}
	m_poolStagingBufferIDs.clear();
#endif

	auto pSyncObjectManager = m_pDevice->GetSyncObjectManager();
	for (auto& semaphore : m_pWaitSemaphores)
	{
		if (!semaphore->preserve)
		{
			pSyncObjectManager->ReturnSemaphore(semaphore->semaphore_id);
		}
	}
	m_pWaitSemaphores.clear();
	m_waitSemaphoreHead = 0;

	return true;
}

bool DrawingCommandManager_Vulkan::CopyBuffer(uint32_t id, const std::shared_ptr<DrawingRawBuffer_Vulkan> src, VkBuffer& dst, const VkBufferCopy& region)
{
	assert(id < m_commandBuffers.size());
	assert(m_commandBuffers[id].m_inUse);
	
#if defined(POOLED_STAGING_BUFFER_VK)
	m_poolStagingBufferIDs.emplace_back(src->m_stagingID);;
#else
	m_pStagingBuffers.emplace_back(src);
#endif
	vkCmdCopyBuffer(m_commandBuffers[id].m_bufferHandle, src->m_buffer, dst, 1, &region);

	return true;
}

bool DrawingCommandManager_Vulkan::CopyBufferToImage(uint32_t id, const std::shared_ptr<DrawingRawBuffer_Vulkan> buffer, VkImage& image, const std::vector<VkBufferImageCopy>& regions)
{
	assert(id < m_commandBuffers.size());
	assert(m_commandBuffers[id].m_inUse);
	
#if defined(POOLED_STAGING_BUFFER_VK)
	m_poolStagingBufferIDs.emplace_back(buffer->m_stagingID);;
#else
	m_pStagingBuffers.emplace_back(buffer);
#endif
	vkCmdCopyBufferToImage(m_commandBuffers[id].m_bufferHandle, buffer->m_buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(regions.size()), regions.data());

	return true;
}

DrawingCommandBuffer_Vulkan DrawingCommandManager_Vulkan::BeginSingleTimeCommands()
{
	for (auto& availableCmdBuffer : m_singleTimeCommandBuffers)
	{
		if (!availableCmdBuffer.m_inUse)
		{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			VK_CheckResult(vkBeginCommandBuffer(availableCmdBuffer.m_bufferHandle, &beginInfo), "vkBeginCommandBuffer");
			availableCmdBuffer.m_inUse = true;

			return availableCmdBuffer;
		}
	}

	// No available one found, allocate new command buffer
	assert(m_singleTimeCommandBuffers.size() < MAX_SINGLE_TIME_COMMAND_BUFFER);

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_singleTimeCommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	VK_CheckResult(vkAllocateCommandBuffers(m_pDevice->m_logicalDevice, &allocInfo, &commandBuffer), "vkAllocateCommandBuffers");
	m_singleTimeCommandBuffers.emplace_back(commandBuffer, static_cast<uint32_t>(m_singleTimeCommandBuffers.size()));

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VK_CheckResult(vkBeginCommandBuffer(commandBuffer, &beginInfo), "vkBeginCommandBuffer");
	m_singleTimeCommandBuffers[m_singleTimeCommandBuffers.size() - 1].m_inUse = true;

	return m_singleTimeCommandBuffers[m_singleTimeCommandBuffers.size() - 1];
}

bool DrawingCommandManager_Vulkan::EndSingleTimeCommands(DrawingCommandBuffer_Vulkan& commandBuffer)
{
	assert(commandBuffer.m_inUse);
	assert(commandBuffer.m_singleTimeIndex < m_singleTimeCommandBuffers.size());

	VK_CheckResult(vkEndCommandBuffer(commandBuffer.m_bufferHandle), "vkEndCommandBuffer");

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer.m_bufferHandle;
	submitInfo.pWaitDstStageMask = 0;

	vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_queue); // This would throttle performance. Equivalent to submitting a fence to a queue and waiting with an infinite timeout for that fence to signal

	m_singleTimeCommandBuffers[commandBuffer.m_singleTimeIndex].m_inUse = false;
	m_singleTimeCommandBuffers[commandBuffer.m_singleTimeIndex].m_inRenderPass = false;

	return true;
}

bool DrawingCommandManager_Vulkan::TransitionImageLayout_Immediate(std::shared_ptr<DrawingRawImage_Vulkan> pImage, const VkImageLayout newLayout)
{
	if (pImage->m_layout == newLayout)
	{
		return false;
	}
	
	assert(newLayout != VK_IMAGE_LAYOUT_UNDEFINED && newLayout != VK_IMAGE_LAYOUT_PREINITIALIZED);

	DrawingCommandBuffer_Vulkan commandBuffer = BeginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = pImage->m_layout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // We do not perform ownership transfer here
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = pImage->m_image;

	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (VK_HasStencilComponent(pImage->m_format))
		{
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = pImage->m_mipLevels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	VK_GetAccessAndStageFromLayout(pImage->m_layout, barrier.srcAccessMask, sourceStage);
	VK_GetAccessAndStageFromLayout(newLayout, barrier.dstAccessMask, destinationStage);

	vkCmdPipelineBarrier(
		commandBuffer.m_bufferHandle,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	bool success = EndSingleTimeCommands(commandBuffer);

	if (success)
	{
		pImage->m_layout = newLayout;
	}

	return success;
}

bool DrawingCommandManager_Vulkan::TransitionImageLayout_Recorded(uint32_t id, std::shared_ptr<DrawingRawImage_Vulkan> pImage, const VkImageLayout newLayout)
{
	assert(id < m_commandBuffers.size());
	return TransitionImageLayout_Recorded(m_commandBuffers[id], pImage, newLayout);
}

bool DrawingCommandManager_Vulkan::TransitionImageLayout_Recorded(const DrawingCommandBuffer_Vulkan& commandBuffer, std::shared_ptr<DrawingRawImage_Vulkan> pImage, const VkImageLayout newLayout)
{
	if (pImage->m_layout == newLayout)
	{
		return false;
	}

	assert(newLayout != VK_IMAGE_LAYOUT_UNDEFINED && newLayout != VK_IMAGE_LAYOUT_PREINITIALIZED);
	assert(commandBuffer.m_inUse);

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = pImage->m_layout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // We do not perform ownership transfer here
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = pImage->m_image;

	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (VK_HasStencilComponent(pImage->m_format))
		{
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = pImage->m_mipLevels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	VK_GetAccessAndStageFromLayout(pImage->m_layout, barrier.srcAccessMask, sourceStage);
	VK_GetAccessAndStageFromLayout(newLayout, barrier.dstAccessMask, destinationStage);

	vkCmdPipelineBarrier(
		commandBuffer.m_bufferHandle,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	pImage->m_layout = newLayout; // Alert: this is in fact not applied immediately

	return true;
}

bool DrawingCommandManager_Vulkan::ClearImageColor(std::shared_ptr<DrawingRawImage_Vulkan> pImage, const float4 clearColor)
{
	VkImageLayout originalLayout = pImage->m_layout;
	// We don't care about the existing content of the image since we are going to clear it
	pImage->m_layout = VK_IMAGE_LAYOUT_UNDEFINED;
	TransitionImageLayout_Immediate(pImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	DrawingCommandBuffer_Vulkan commandBuffer = BeginSingleTimeCommands();

	VkClearColorValue color = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };
	VkImageSubresourceRange imageRange = {};
	imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Alert: these parameters do not cover all cases
	imageRange.levelCount = pImage->m_mipLevels;
	imageRange.layerCount = 1;
	imageRange.baseArrayLayer = 0;
	imageRange.baseMipLevel = 0;

	vkCmdClearColorImage(commandBuffer.m_bufferHandle, pImage->m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &color, 1, &imageRange);

	EndSingleTimeCommands(commandBuffer);

	if (originalLayout != VK_IMAGE_LAYOUT_UNDEFINED && originalLayout != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && originalLayout != VK_IMAGE_LAYOUT_PREINITIALIZED)
	{
		TransitionImageLayout_Immediate(pImage, originalLayout);
	}

	return true;
}

bool DrawingCommandManager_Vulkan::IsCommandBufferInUse(uint32_t id) const
{
	assert(id < m_commandBuffers.size());
	return m_commandBuffers[id].m_inUse && !m_commandBuffers[id].m_inExecution;
}

bool DrawingCommandManager_Vulkan::IsCommandBufferInRenderPass(uint32_t id) const
{
	assert(id < m_commandBuffers.size());
	return m_commandBuffers[id].m_inRenderPass;
}

DrawingCommandSyncObejctManager_Vulkan::DrawingCommandSyncObejctManager_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, uint32_t initialSemaphoreCount, uint32_t initialFenceCount)
	: m_pDevice(pDevice)
{
	m_pDevice = pDevice;
	CreateNewSemaphore(initialSemaphoreCount);
	CreateNewFence(initialFenceCount);
}

DrawingCommandSyncObejctManager_Vulkan::~DrawingCommandSyncObejctManager_Vulkan()
{
	for (size_t i = 0; i < m_semaphores.size(); ++i)
	{
		vkDestroySemaphore(m_pDevice->m_logicalDevice, m_semaphores[i], nullptr);
	}
	for (size_t i = 0; i < m_fences.size(); ++i)
	{
		vkDestroyFence(m_pDevice->m_logicalDevice, m_fences[i], nullptr);
	}

	m_semaphores.clear();
	m_fences.clear();
	m_semaphoreAvailability.clear();
	m_fenceAvailability.clear();
}

VkSemaphore DrawingCommandSyncObejctManager_Vulkan::RequestSemaphore(uint32_t id) const
{
	assert(id < m_semaphores.size());
	return m_semaphores[id];
}

VkFence DrawingCommandSyncObejctManager_Vulkan::RequestFence(uint32_t id) const
{
	assert(id < m_fences.size());
	return m_fences[id];
}

VkEvent DrawingCommandSyncObejctManager_Vulkan::RequestEvent(uint32_t id) const
{
	assert(id < m_events.size());
	return m_events[id];
}

bool DrawingCommandSyncObejctManager_Vulkan::IsSemaphoreBeingUsed(uint32_t id) const
{
	assert(id < m_semaphores.size());
	return !m_semaphoreAvailability.at(id);
}

uint32_t DrawingCommandSyncObejctManager_Vulkan::RequestSemaphoreID()
{
	for (auto& semaphore : m_semaphoreAvailability)
	{
		if (semaphore.second)
		{
			semaphore.second = false;
			return semaphore.first;
		}
	}

	CreateNewSemaphore(1);
	m_semaphoreAvailability[static_cast<uint32_t>(m_semaphores.size() - 1)] = false;
	return static_cast<uint32_t>(m_semaphores.size() - 1);
}

uint32_t DrawingCommandSyncObejctManager_Vulkan::RequestFenceID()
{
	for (auto& fence : m_fenceAvailability)
	{
		if (fence.second)
		{
			fence.second = false;
			return fence.first;
		}
	}

	CreateNewFence(1);
	m_fenceAvailability[static_cast<uint32_t>(m_fences.size() - 1)] = false;
	return static_cast<uint32_t>(m_fences.size() - 1);
}

uint32_t DrawingCommandSyncObejctManager_Vulkan::RequestEventID()
{
	for (auto& event : m_eventAvailability)
	{
		if (event.second)
		{
			event.second = false;
			return event.first;
		}
	}

	CreateNewEvent(1);
	m_eventAvailability[static_cast<uint32_t>(m_events.size() - 1)] = false;
	return static_cast<uint32_t>(m_events.size() - 1);
}

void DrawingCommandSyncObejctManager_Vulkan::ReturnSemaphore(uint32_t id)
{
	assert(id < m_semaphores.size());
	m_semaphoreAvailability[id] = true;
}

void DrawingCommandSyncObejctManager_Vulkan::ReturnFence(uint32_t id)
{
	assert(id < m_fences.size());
	m_fenceAvailability[id] = true;
}

void DrawingCommandSyncObejctManager_Vulkan::ReturnEvent(uint32_t id)
{
	assert(id < m_events.size());
	m_eventAvailability[id] = true;
}

bool DrawingCommandSyncObejctManager_Vulkan::CreateNewSemaphore(uint32_t count)
{
	assert(m_semaphores.size() + count <= MAX_SEMAPHORE);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	for (uint32_t i = 0; i < count; ++i)
	{
		VkSemaphore newSemaphore;
		VK_CheckResult(vkCreateSemaphore(m_pDevice->m_logicalDevice, &semaphoreInfo, nullptr, &newSemaphore), "vkCreateSemaphore");	
		m_semaphores.emplace_back(newSemaphore);
		m_semaphoreAvailability.emplace(static_cast<uint32_t>(m_semaphores.size() - 1), true);
	}

	return true;
}

bool DrawingCommandSyncObejctManager_Vulkan::CreateNewFence(uint32_t count, bool signaled)
{
	assert(m_fences.size() + count <= MAX_FENCE);

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

	for (uint32_t i = 0; i < count; ++i)
	{
		VkFence newFence;
		VK_CheckResult(vkCreateFence(m_pDevice->m_logicalDevice, &fenceInfo, nullptr, &newFence), "vkCreateFence");
		m_fenceAvailability.emplace(static_cast<uint32_t>(m_fences.size()), true);
		m_fences.emplace_back(newFence);
	}

	return true;
}

bool DrawingCommandSyncObejctManager_Vulkan::CreateNewEvent(uint32_t count)
{
	assert(m_events.size() + count <= MAX_EVENT);

	VkEventCreateInfo eventInfo = {};
	eventInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;

	for (uint32_t i = 0; i < count; ++i)
	{
		VkEvent newEvent;
		VK_CheckResult(vkCreateEvent(m_pDevice->m_logicalDevice, &eventInfo, nullptr, &newEvent), "vkCreateEvent");
		m_eventAvailability.emplace(static_cast<uint32_t>(m_events.size()), true);
		m_events.emplace_back(newEvent);
	}

	return true;
}