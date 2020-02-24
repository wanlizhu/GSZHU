#pragma once
#include <vulkan/vulkan.h>
#include <spirv_cross.hpp>
#include <memory>
#include <array>

#include "Matrix.h"
#include "Vector.h"
#include "DrawingRawResource.h"

#include "DrawingUtil_Vulkan.h"
#include "DrawingExtensionWrangler_Vulkan.h"
#include "DrawingUploadAllocator_Vulkan.h"


namespace Engine
{
	struct RawImageCreateInfo_Vulkan;
	struct SwapchainCreateInfo_Vulkan
	{
		VkSurfaceKHR					surface;
		VkSurfaceFormatKHR				surfaceFormat;
		VkPresentModeKHR				presentMode;
		VkExtent2D						swapExtent;
		SwapchainSupportDetails_Vulkan	swapchainSupport;
		QueueFamilyIndices_Vulkan		queueFamilyIndices;
		uint32_t						maxFramesInFlight;
	};

	typedef struct _VulkanSubresourceData
	{
		const void* pData;
		uint64_t	RowSize;
		uint64_t	SliceSize;
		uint64_t	Offset;
		uint32_t	FormatBytes;
	} VulkanSubresourceData;

	enum UniformBufferType_Vulkan
	{
		eUniformBuffer_UNDEFINED = 0,
		eUniformBuffer_UNIFORM,
		eUniformBuffer_PUSH_CONSTANT,
		eUniformBuffer_TYPE_COUNT
	};

	class DrawingDevice_Vulkan;
	class DrawingRawVertexFormat_Vulkan : public DrawingRawVertexFormat
	{
	public:
		DrawingRawVertexFormat_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, const std::vector<VkVertexInputBindingDescription>& bindingDesc, const std::vector<VkVertexInputAttributeDescription>& attribDesc);
		virtual ~DrawingRawVertexFormat_Vulkan() = default;

		const VkPipelineVertexInputStateCreateInfo* Get() const;

	private:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::vector<VkVertexInputBindingDescription> m_bindingDescs;
		std::vector<VkVertexInputAttributeDescription> m_attribDescs;
		VkPipelineVertexInputStateCreateInfo m_inputInfo;
	};

	class DrawingRawBuffer_Vulkan
	{
	public:
		DrawingRawBuffer_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, const VkBufferUsageFlags usage, const VmaMemoryUsage memoryUsage, const VkDeviceSize size, const VkDeviceSize stride = 0);
		virtual ~DrawingRawBuffer_Vulkan();

		VkDeviceSize GetSize() const;
		VkDeviceSize GetStride() const;
		void* GetMappedPointer();
		void UnmapMemory();

		bool isValid() const;
		void Free();

	public:
		VkBuffer m_buffer;
		VmaAllocation m_allocation;
#if defined(POOLED_STAGING_BUFFER_VK)
		uint32_t m_stagingID = -1;
#endif

	protected:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::shared_ptr<DrawingUploadAllocator_Vulkan> m_pAllocator;
		VkDeviceSize m_sizeInBytes;
		VkDeviceSize m_strideInBytes;

		bool m_isValid;
	};

	class DrawingRawVertexBuffer_Vulkan : public DrawingRawVertexBuffer
	{
	public:
		DrawingRawVertexBuffer_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, const void* pData, const VkDeviceSize size, const VkDeviceSize stride);
		virtual ~DrawingRawVertexBuffer_Vulkan();

		VkBuffer GetBuffer() const;
		VkDeviceSize GetSize() const;
		VkDeviceSize GetStride() const;

		void* MapResource();
		void UnmapResource();

		void Free();

	private:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::shared_ptr<DrawingUploadAllocator_Vulkan> m_pAllocator;
		std::shared_ptr<DrawingRawBuffer_Vulkan> m_pBufferImpl;
	};

	class DrawingRawIndexBuffer_Vulkan : public DrawingRawIndexBuffer
	{
	public:
		DrawingRawIndexBuffer_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, const void* pData, VkDeviceSize size, VkDeviceSize stride);
		virtual ~DrawingRawIndexBuffer_Vulkan();

		VkBuffer GetBuffer() const;
		VkDeviceSize GetSize() const;
		VkDeviceSize GetStride() const;
		VkIndexType GetFormat() const;

		void* MapResource();
		void UnmapResource();

		void Free();

	private:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::shared_ptr<DrawingUploadAllocator_Vulkan> m_pAllocator;
		std::shared_ptr<DrawingRawBuffer_Vulkan> m_pBufferImpl;
		VkIndexType m_format;
	};

	class DrawingRawUniformBuffer_Vulkan;
	class DrawingRawShader_Vulkan : public DrawingRawShader_Common
	{
	public:
		DrawingRawShader_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, VkShaderModule shaderModule, const VkShaderStageFlagBits stage, const char* entry = "main");
		virtual ~DrawingRawShader_Vulkan();

		VkShaderModule GetModule() const;
		const char* EntryName() const;
		VkShaderStageFlagBits GetStage() const;

		void BuildResourceBindingTable(DrawingRawShader::DrawingRawShaderType type);

	public:
		// The raw code is for reflection
		std::vector<char> m_rawCode;

	private:
		void ProcessVariables(std::shared_ptr<std::string> pName, const spirv_cross::Compiler& comp, const spirv_cross::Resource& resource);

	protected:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		VkShaderModule m_shaderModule;
		const char* m_entryName;
		VkShaderStageFlagBits m_stage;
	};

	class DrawingRawVertexShader_Vulkan : public DrawingRawVertexShader
	{
	public:
		DrawingRawVertexShader_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<std::string> pShaderName, VkShaderModule shaderModule, std::vector<char>& rawCode, const char* entry = "main");
		virtual ~DrawingRawVertexShader_Vulkan() = default;

		VkShaderModule Get() const;
		std::shared_ptr<DrawingRawShader_Vulkan> GetShaderImpl() const;
		VkShaderStageFlagBits GetStage() const;

	private:
		std::shared_ptr<DrawingRawShader_Vulkan> m_pShaderImpl;
		VkShaderStageFlagBits m_stage;
	};

	class DrawingRawFragmentShader_Vulkan : public DrawingRawPixelShader
	{
	public:
		DrawingRawFragmentShader_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<std::string> pShaderName, VkShaderModule shaderModule, std::vector<char>& rawCode, const char* entry = "main");
		virtual ~DrawingRawFragmentShader_Vulkan() = default;

		VkShaderModule Get() const;
		std::shared_ptr<DrawingRawShader_Vulkan> GetShaderImpl() const;
		VkShaderStageFlagBits GetStage() const;

	private:
		std::shared_ptr<DrawingRawShader_Vulkan> m_pShaderImpl;
		VkShaderStageFlagBits m_stage;
	};

	class DrawingRawImage_Vulkan
	{
	public:
		DrawingRawImage_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const RawImageCreateInfo_Vulkan& createInfo);
		DrawingRawImage_Vulkan(const VkImage targetImage, const VkImageView targetView, const VkExtent2D targetExtent, const VkFormat targetFormat); // Exclusively for render target creation
		virtual ~DrawingRawImage_Vulkan();

		bool isValid() const;
		std::shared_ptr<DrawingUploadAllocator_Vulkan> GetAllocator() const
		{
			return m_pAllocator;
		}
		void Free();

	public:
		VkImage			m_image;
		VkImageView		m_imageView;
		VkImageLayout	m_layout;
		VmaAllocation	m_allocation;
		VkExtent3D		m_extent;
		VkFormat		m_format;
		uint32_t		m_mipLevels;

	protected:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::shared_ptr<DrawingUploadAllocator_Vulkan> m_pAllocator;
		bool m_isValid;
	};

	class DrawingRawTarget_Vulkan : public DrawingRawTarget
	{
	public:
		DrawingRawTarget_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<DrawingRawImage_Vulkan> rawImage);
		virtual ~DrawingRawTarget_Vulkan() = default;

		virtual ETargetType GetTargetType() const;
		virtual std::shared_ptr<DrawingRawImage_Vulkan> GetRenderTarget() const;
		VkImageView GetTargetView() const;
		VkFormat GetTargetFormat() const;
		VkExtent2D GetExtent() const;

	protected:
		DrawingRawTarget_Vulkan() {};
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::shared_ptr<DrawingRawImage_Vulkan> m_pTargetImpl;
	};

	class DrawingRawDepthTarget_Vulkan : public DrawingRawTarget_Vulkan
	{
	public:
		DrawingRawDepthTarget_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, RawImageCreateInfo_Vulkan& createInfo);

		ETargetType GetTargetType() const override;
	};

	class DrawingRawFragmentTarget_Vulkan : public DrawingRawTarget_Vulkan
	{
	public:
		DrawingRawFragmentTarget_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice);
		DrawingRawFragmentTarget_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<DrawingRawImage_Vulkan> pImage);
		virtual ~DrawingRawFragmentTarget_Vulkan() = default;

		uint32_t GetCurrentIndex() const;

	protected:
		uint32_t m_bufferIndex;
	};

	// The 'FragmentTarget' for the swapchain is the current render target the swapchain is using
	class DrawingRawSwapchain_Vulkan : public DrawingRawFragmentTarget_Vulkan
	{
	public:
		DrawingRawSwapchain_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, SwapchainCreateInfo_Vulkan& createInfo);
		virtual ~DrawingRawSwapchain_Vulkan();

		void Free();
		bool CreateFramebuffers(VkImageView depthImageView, VkRenderPass renderPass);
		bool InitializeTargetLayout(); // This is not recommended to call manually, better way is to let the render pass automatically complete the transition

		VkResult UpdateBackBuffer(VkSemaphore semaphore, uint64_t timeout);
		ETargetType GetTargetType() const override;
		size_t GetTargetCount() const;
		VkFramebuffer GetTargetFramebuffer() const;
		VkImageView GetTargetViewAt(uint32_t index) const;

		VkResult Present(const std::vector<std::shared_ptr<SignalSempahore_Vulkan>>& waitSemaphores, uint32_t syncInterval);

	private:
		VkSwapchainKHR m_swapchain;
		VkQueue m_presentQueue;
		std::vector<std::shared_ptr<DrawingRawTarget_Vulkan>> m_pTargetArray;
		std::vector<VkFramebuffer> m_swapchainFramebuffers;
		bool m_isValid = false;
	};

	class DrawingRawEffect_Vulkan : public DrawingRawEffect
	{
	public:
		DrawingRawEffect_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<std::string> pEffectName);
		virtual ~DrawingRawEffect_Vulkan() = default;

		std::shared_ptr<DrawingDescriptorSetLayout_Vulkan> GetDescriptorSetLayoutObject() const;

	protected:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::shared_ptr<DrawingDescriptorSetLayout_Vulkan> m_pDescriptorSetLayout;
	};

	class DrawingRawTexture_Vulkan;
	class DrawingRawTexture2D_Vulkan;
	class DrawingRawShaderEffect_Vulkan : public DrawingRawEffect_Vulkan
	{
	public:
		DrawingRawShaderEffect_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<std::string> pEffectName, std::shared_ptr<DrawingRawVertexShader_Vulkan> pVertexShader, std::shared_ptr<DrawingRawFragmentShader_Vulkan> pFragmentShader, uint32_t maxSetsCount);
		virtual ~DrawingRawShaderEffect_Vulkan() = default;

		void Apply() override;
		void Terminate() override;

		std::shared_ptr<DrawingRawShader> GetRawShader(DrawingRawShader::DrawingRawShaderType type) const;

		const VkPushConstantRange* GetPushConstantRanges() const;
		size_t PushConstantSize() const;

		void DescriptorDirty();
		void ResetDescriptorUpdateIndex(); // TODO: find a better solution

	private:
		static constexpr const int32_t EMPTY_SLOT = -1;

		struct SUniformBuffer
		{
			std::shared_ptr<std::string> mpName = nullptr;
			uint32_t mSizeInBytes = 0;

			std::array<uint32_t, DrawingRawShader::RawShader_Count> mStartSlot;
			std::shared_ptr<DrawingRawConstantBuffer> mpUB;
			SUniformBuffer()
			{
				mStartSlot.fill(EMPTY_SLOT);
			}
		};

		struct SParamVar
		{
			std::shared_ptr<DrawingParameter> mpParam = nullptr;
			uint32_t mSizeInBytes = 0;

			std::array<uint32_t, DrawingRawShader::RawShader_Count> mOffset;
			std::array<std::shared_ptr<DrawingRawConstantBuffer>, DrawingRawShader::RawShader_Count> mpCB;

			SParamVar()
			{
				mOffset.fill(0);
			}

			void UpdateValues(void);
		};

		struct SParamRes
		{
			std::shared_ptr<DrawingParameter> mpParam = nullptr;
			uint32_t mCount = 0;

			std::array<uint32_t, DrawingRawShader::RawShader_Count> mStartSlot;

			SParamRes()
			{
				mStartSlot.fill(EMPTY_SLOT);
			}
		};

	private:
		void CollectVariables(DrawingDevice::ConstBufferPropTable& cbPropTable, const DrawingRawShader_Vulkan* pShader);
		void SortVariables(DrawingDevice::ConstBufferPropTable& cbPropTable);
		void BuildUBPropTable(DrawingDevice::ConstBufferPropTable& cbPropTable, const DrawingRawShader_Vulkan* pShader);

		void LoadShaderInfo(const std::shared_ptr<DrawingRawShader_Vulkan> pShader, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount);
		void LoadUniformBufferFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount);
		void LoadStorageBufferFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount);
		void LoadSubpassInputFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount);
		void LoadStorageImageFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount);
		void LoadCombinedImageSamplerFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount);
		void LoadAccelerationStructureFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount);
		void LoadSampledImageFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount);
		void LoadSamplerFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType, uint32_t maxSetsCount);
		void LoadPushConstantFromShader(const spirv_cross::Compiler& comp, const spirv_cross::ShaderResources& res, const DrawingRawShader::DrawingRawShaderType shaderType);

		bool CreateDescriptorSetLayout();
		bool CreateDescriptorPool(uint32_t maxSetsCount);
		bool AllocateDescriptorSets(uint32_t setsCount);

		void UpdateParameterValues();
		void UpdateUniformBuffers(); // This includes the update of the push constants
		void UpdateDescriptorSets();

		void BindUniformBuffer(DrawingDevice::ConstBufferPropTable& cbPropTable, const DrawingRawShader_Vulkan* pShader, const DrawingRawShader::DrawingRawShaderType shaderType);
		void GenerateParameters(const DrawingRawShader_Vulkan* pShader, const DrawingRawShader::DrawingRawShaderType shaderType);

		void CheckAndAddResource(const DrawingRawShader_Common::ShaderResourceDesc& desc, uint32_t paramType, const DrawingRawShader::DrawingRawShaderType shaderType, std::unordered_map<std::shared_ptr<std::string>, SParamRes>& resTable) const;
		void BuildTextureTable(const DrawingRawShader_Vulkan* pShader, const DrawingRawShader::DrawingRawShaderType shaderType);

	private:
		std::array<std::shared_ptr<DrawingRawShader>, DrawingRawShader::RawShader_Count> m_shaderBlocks;
		uint32_t m_descriptorPoolID;
		std::vector<VkDescriptorSet> m_descriptorSets;

		std::unordered_map<std::shared_ptr<std::string>, SUniformBuffer> m_uniformBufferTable;
		std::unordered_map<std::shared_ptr<std::string>, SParamVar> m_varTable;
		std::unordered_map<std::shared_ptr<std::string>, SParamRes> m_texTable;

		std::vector<VkDescriptorSetLayoutBinding> m_layoutBindings;
		std::vector<VkPushConstantRange> m_pushConstantRanges;
		std::vector<VkDescriptorPoolSize> m_poolSizes;

		bool m_descriptorDirty;
		uint32_t m_descriptorSetUpdateIndex = 0;

		std::vector<std::shared_ptr<DrawingRawUniformBuffer_Vulkan>> m_pUniformBuffers;
#if defined(POOLED_TEXTURE_VK)
		std::vector<std::pair<uint32_t, const std::shared_ptr<DrawingRawTexture_Vulkan>>> m_pSampledImages;
#else
		std::vector<std::pair<uint32_t, const DrawingRawTexture_Vulkan*>> m_pSampledImages; // This is a temp solution to record layoutParameterIndex
#endif
	};

	class DrawingRawPipelineState_Vulkan : public DrawingRawPipelineState
	{
	public:
		DrawingRawPipelineState_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, std::shared_ptr<DrawingRawEffect_Vulkan> pEffect, VkGraphicsPipelineCreateInfo& pipelineInfo);
		virtual ~DrawingRawPipelineState_Vulkan();

		VkPipeline GetPipeline() const;
		std::shared_ptr<DrawingRawEffect_Vulkan> GetEffect() const;

	private:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::shared_ptr<DrawingRawEffect_Vulkan> m_pEffect;
		VkPipeline m_pipeline;
	};

	class DrawingRawUniformBuffer_Vulkan : public DrawingRawConstantBuffer
	{
	public:
		DrawingRawUniformBuffer_Vulkan(std::shared_ptr<DrawingDevice_Vulkan> pDevice, uint32_t sizeInBytes, uint32_t layoutParameterIndex);
		virtual ~DrawingRawUniformBuffer_Vulkan();

		void UpdateToHardware() override;

		void MarkAsUniformBuffer();
		void MarkAsPushConstant(VkShaderStageFlags appliedStage);
		void UnmarkPushConstant();

		const UniformBufferType_Vulkan GetType() const;
		std::shared_ptr<DrawingRawBuffer_Vulkan> GetUniformBufferImpl() const;
		const uint32_t GetBindingIndex() const;

	private:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		uint32_t m_layoutParameterIndex;
		
		UniformBufferType_Vulkan m_eType;
		VkShaderStageFlags m_appliedShaderStage;

		// Push constants don't need these, these are for regular uniform buffer
		std::shared_ptr<DrawingUploadAllocator_Vulkan> m_pAllocator;
		std::shared_ptr<DrawingRawBuffer_Vulkan> m_pBufferImpl;
		void* m_hostData;	  // Alert: exposing this raw data pointer could be a bad practice
		bool  m_memoryMapped; // Alert: persisting memory mapping could cause error sometimes
	};

	class DrawingRawTexture_Vulkan : public DrawingRawTexture
	{
	public:
		DrawingRawTexture_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const RawImageCreateInfo_Vulkan& createInfo, std::vector<VulkanSubresourceData>& data, uint32_t dataSize);
#if defined(POOLED_TEXTURE_VK)
		DrawingRawTexture_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const RawImageCreateInfo_Vulkan& createInfo);
#endif
		virtual ~DrawingRawTexture_Vulkan() = default;

		std::shared_ptr<DrawingRawImage_Vulkan> GetResource() const;
		void CopySubresource(const std::vector<VulkanSubresourceData>& data, uint32_t dataSize, const bool immediateCopy = false, const bool readAfterCopy = false);

		void* MapResource();
		void UnmapResource();

	public:
#if defined(POOLED_TEXTURE_VK)
		uint32_t m_copyFinishedSemaphoreID;
		uint32_t m_poolID;
#endif

	protected:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::shared_ptr<DrawingRawImage_Vulkan> m_pResource;
		uint32_t m_layoutParameterIndex; // This has not been used
	};

	class DrawingRawTexture2D_Vulkan : public DrawingRawTexture_Vulkan
	{
	public:
		DrawingRawTexture2D_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const RawImageCreateInfo_Vulkan& createInfo, std::vector<VulkanSubresourceData>& data, uint32_t dataSize);
		virtual ~DrawingRawTexture2D_Vulkan() = default;
	};

#if defined(POOLED_TEXTURE_VK)
	class DrawingRawTexturePool_Vulkan
	{
	public:
		DrawingRawTexturePool_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, uint32_t initialSize, const RawImageCreateInfo_Vulkan& textureTemplate, uint32_t poolID);
		virtual ~DrawingRawTexturePool_Vulkan();

		const uint32_t GetPoolID() const;
		const uint32_t RequestTextureID();
		const uint32_t LastRequestedTextureID();
		const std::shared_ptr<DrawingRawTexture_Vulkan> RequestTexture(uint32_t id) const;
		void ResetPool(bool deepReset = false);
		bool TransitionPoolTextures(VkImageLayout destLayout);

	private:
		bool AllocateNewPooledTexture(uint32_t count);
		bool FreeTextures();

	private:
		const uint32_t MAX_POOL_SIZE = 64;
		const RawImageCreateInfo_Vulkan POOL_TEMPLATE;

		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		uint32_t m_poolID; // This ID should represent the type of texture it's holding (it should match the texture's pool ID)
		uint32_t m_distributedID;
		std::vector<std::shared_ptr<DrawingRawTexture_Vulkan>> m_pooledTextures;
	};
#endif

#if defined(POOLED_STAGING_BUFFER_VK)
	class DrawingRawStagingBufferPool_Vulkan
	{
	public:
		DrawingRawStagingBufferPool_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice);
		virtual ~DrawingRawStagingBufferPool_Vulkan();

		uint32_t RequestStagingBufferID(uint32_t desiredSize);
		std::shared_ptr<DrawingRawBuffer_Vulkan> RequestStagingBuffer(uint32_t id) const;
		void ReturnStagingBuffer(uint32_t id);

	private:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		const uint32_t MAX_POOL_SIZE = 128;
		std::vector<std::shared_ptr<DrawingRawBuffer_Vulkan>> m_stagingBuffers;
		std::unordered_map<uint32_t, bool> m_bufferAvailability;
	};
#endif
}