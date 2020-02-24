#pragma once
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

#include "DrawingDevice.h"
#include "PerformanceTestParameters_Vulkan.h"

namespace Engine
{
	class DrawingCommandBuffer_Vulkan
	{
	public:
		DrawingCommandBuffer_Vulkan(const VkCommandBuffer cmdBuffer)
			: m_bufferHandle(cmdBuffer), m_inUse(false), m_inRenderPass(false), m_inExecution(false), m_singleTimeIndex(-1) {}
		DrawingCommandBuffer_Vulkan(const VkCommandBuffer cmdBuffer, const uint32_t singleTimeIndex)
			: m_bufferHandle(cmdBuffer), m_inUse(false), m_inRenderPass(false), m_inExecution(false), m_singleTimeIndex(singleTimeIndex) {}

		VkCommandBuffer m_bufferHandle;
		bool m_inUse;
		bool m_inRenderPass;
		bool m_inExecution;
#if defined(CONTROLLED_SUBMIT_RATE_VK)
		bool m_awaitSubmit = false;
#endif

		uint32_t m_singleTimeIndex;
	};

	enum WaitSemaphoreType_Vulkan
	{
		eWaitType_Undefined = 0,
		eWaitType_Copy
	};

	typedef struct _WaitSemaphore_Vulkan
	{
		_WaitSemaphore_Vulkan(const uint32_t id, const VkSemaphore semaphoreHandle, const VkPipelineStageFlags stage, bool shouldPreserve = false, WaitSemaphoreType_Vulkan waitType = eWaitType_Undefined)
			: semaphore_id(id), semaphore(semaphoreHandle), waitStage(stage), preserve(shouldPreserve), type(waitType) {}

		uint32_t				 semaphore_id;
		VkSemaphore				 semaphore;
		VkPipelineStageFlags	 waitStage;
		bool					 preserve;
		WaitSemaphoreType_Vulkan type;
	} WaitSemaphore_Vulkan;

	typedef struct _SignalSemaphore_Vulkan
	{
		_SignalSemaphore_Vulkan(const uint32_t id, const VkSemaphore semaphoreHandle, WaitSemaphoreType_Vulkan waitType = eWaitType_Undefined)
			: semaphore_id(id), semaphore(semaphoreHandle), type(waitType) {}

		uint32_t				 semaphore_id;
		VkSemaphore				 semaphore;
		WaitSemaphoreType_Vulkan type;
	} SignalSempahore_Vulkan;

	// Forward declaration
	class DrawingRawImage_Vulkan;
	class DrawingDevice_Vulkan;
	class DrawingRawBuffer_Vulkan;
	class DrawingCommandManager_Vulkan
	{
	public:
		DrawingCommandManager_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const VkQueue queue, EDrawingCommandListType type);
		virtual ~DrawingCommandManager_Vulkan();

		EDrawingCommandListType GetCommandBufferType() const;

		void Destroy();
		void FreeBuffers();
		bool ReleaseReservedResources(); // This must be called at least once every frame

		bool CreateCommandPool(uint32_t queueFamilyIndex);
		bool AllocateCommandBuffers(uint32_t size);

		uint32_t BeginNewCommandBuffer();
		void AddSemaphoreToWaitlist(const std::shared_ptr<WaitSemaphore_Vulkan> pWaitSemaphore);
		bool SubmitCommandBuffers(const VkFence fence, const std::vector<std::shared_ptr<SignalSempahore_Vulkan>>& signalSemaphores, bool relaxed = true);

		// These functions should ONLY be called in GRAPHICS(DIRECT) type command manager
		bool BindVertexBuffer(uint32_t id, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* vertexBuffer, const VkDeviceSize* offsets);
		bool BindIndexBuffer(uint32_t id, const VkBuffer indexBuffer, const VkDeviceSize offset, VkIndexType type);
		bool BeginRenderPass(uint32_t id, const VkRenderPass renderPass, const VkFramebuffer frameBuffer, const VkExtent2D& swapchainExtent, const float4& clearColor = { 0.0f, 0.0f, 0.0f, 1.0f });
		bool BindPipeline(uint32_t id, VkPipelineBindPoint bindPoint, VkPipeline pipeline);
		void BindPipelineLayout(const VkPipelineLayout pipelineLayout);
		bool UpdatePushConstant(uint32_t id, VkShaderStageFlags shaderStage, uint32_t size, const void* pData, uint32_t offset = 0);
		bool BindDescriptorSets(uint32_t id, const VkPipelineBindPoint bindPoint, const std::vector<VkDescriptorSet> descriptorSets, uint32_t firstSet = 0);
		bool DrawPrimitiveIndexed(uint32_t id, uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstIndex = 0, uint32_t vertexOffset = 0, uint32_t firstInstance = 0);
		bool DrawPrimitiveInstanced(uint32_t id, uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset);
		bool EndRenderPass(uint32_t id);
		bool TransitionImageLayout_Immediate(std::shared_ptr<DrawingRawImage_Vulkan> pImage, const VkImageLayout newLayout);
		bool TransitionImageLayout_Recorded(uint32_t id, std::shared_ptr<DrawingRawImage_Vulkan> pImage, const VkImageLayout newLayout);
		bool TransitionImageLayout_Recorded(const DrawingCommandBuffer_Vulkan& commandBuffer, std::shared_ptr<DrawingRawImage_Vulkan> pImage, const VkImageLayout newLayout);
		// It's not recommended to clear an image manually
		bool ClearImageColor(std::shared_ptr<DrawingRawImage_Vulkan> pImage, const float4 clearColor);

		// These functions are RECOMMENDED to be called in TRANSFER (COPY) type command manager (if enabled)
		bool CopyBuffer(uint32_t id, const std::shared_ptr<DrawingRawBuffer_Vulkan> src, VkBuffer& dst, const VkBufferCopy& region);
		bool CopyBufferToImage(uint32_t id, const std::shared_ptr<DrawingRawBuffer_Vulkan> buffer, VkImage& image, const std::vector<VkBufferImageCopy>& regions);

		// These commands get executed immediately but throttle performance, use as less as possible
		DrawingCommandBuffer_Vulkan BeginSingleTimeCommands();
		bool EndSingleTimeCommands(DrawingCommandBuffer_Vulkan& commandBuffer);

		bool IsCommandBufferInUse(uint32_t id) const;
		bool IsCommandBufferInRenderPass(uint32_t id) const;

#if defined(CONTROLLED_SUBMIT_RATE_VK)
#if defined(ENABLE_TRANSFER_QUEUE_VK)
	public:
		bool t_requiresCopyWait = false;
#endif
#endif

	private:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		VkQueue m_queue;
		EDrawingCommandListType m_type;
		VkPipelineLayout m_pipelineLayout;
		bool m_isValid;

		// Protection from resource overflow; Normally we should not hold too many command buffers in one frame
		const uint32_t MAX_COMMAND_BUFFER = 128;
		const uint32_t MAX_SINGLE_TIME_COMMAND_BUFFER = 16;	

		// Regular commands
		VkCommandPool m_commandPool;
		std::vector<DrawingCommandBuffer_Vulkan> m_commandBuffers;

		// Single time commands
		VkCommandPool m_singleTimeCommandPool;
		std::vector<DrawingCommandBuffer_Vulkan> m_singleTimeCommandBuffers;

		// Reserved resources
		uint32_t m_waitSemaphoreHead = 0;
		std::vector<std::shared_ptr<WaitSemaphore_Vulkan>> m_pWaitSemaphores;
		std::vector<std::shared_ptr<DrawingRawBuffer_Vulkan>> m_pStagingBuffers;
#if defined(POOLED_STAGING_BUFFER_VK)
		std::vector<uint32_t> m_poolStagingBufferIDs;
#endif
#if defined(CONTROLLED_SUBMIT_RATE_VK)
		std::vector<std::shared_ptr<SignalSempahore_Vulkan>> m_signalSemaphores;
#endif
#if defined(COMPACT_COMMAND_BUFFER_VK)
		uint32_t m_stackedSubmissionTimes = 0;
#endif
	};

	class DrawingCommandSyncObejctManager_Vulkan
	{
	public:
		DrawingCommandSyncObejctManager_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, uint32_t initialSemaphoreCount = 0, uint32_t initialFenceCount = 0);
		~DrawingCommandSyncObejctManager_Vulkan();

		VkSemaphore RequestSemaphore(uint32_t id) const;
		VkFence RequestFence(uint32_t id) const;
		VkEvent RequestEvent(uint32_t id) const;

		bool IsSemaphoreBeingUsed(uint32_t id) const;

		uint32_t RequestSemaphoreID();
		uint32_t RequestFenceID();
		uint32_t RequestEventID();

		void ReturnSemaphore(uint32_t id);
		void ReturnFence(uint32_t id);
		void ReturnEvent(uint32_t id);

	private:
		bool CreateNewSemaphore(uint32_t count);
		bool CreateNewFence(uint32_t count, bool signaled = true);
		bool CreateNewEvent(uint32_t count);

	private:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::vector<VkSemaphore> m_semaphores;
		std::vector<VkFence> m_fences;
		std::vector<VkEvent> m_events;

		std::unordered_map<uint32_t, bool> m_semaphoreAvailability;
		std::unordered_map<uint32_t, bool> m_fenceAvailability;
		std::unordered_map<uint32_t, bool> m_eventAvailability;

		const uint32_t MAX_SEMAPHORE = 128; // This is a preset value, if it's not large enough then change it
		const uint32_t MAX_FENCE = 16; // If we are using too many fences, something must be wrong
		const uint32_t MAX_EVENT = 64;
	};
}