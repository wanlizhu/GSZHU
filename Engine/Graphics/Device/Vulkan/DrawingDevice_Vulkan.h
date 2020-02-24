#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <string>

#include "DrawingDevice.h"

#include "DrawingExtensionWrangler_Vulkan.h"
#include "DrawingCommandManager_Vulkan.h"
#include "DrawingUploadAllocator_Vulkan.h"
#include "DrawingDescriptorAllocator_Vulkan.h"
#include "DrawingRawResource_Vulkan.h"
#include "DrawingUtil_Vulkan.h"
#include "PerformanceTestParameters_Vulkan.h"


namespace Engine
{
	class DrawingDevice_Vulkan : public DrawingDevice
	{
	public:

		const std::vector<const char*> m_validationLayers =
		{
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> m_deviceExtensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		const VkPhysicalDeviceFeatures m_deviceFeatures =
		{
			// Add required features here
			// E.g. Anisotropic Filtering
		};

		const VkPresentModeKHR m_optimalPresentModes[4] =
		{
			// The sequence represents their priority
			VK_PRESENT_MODE_IMMEDIATE_KHR,	// Unlimited frame rate (on Radeon WX5100)
			VK_PRESENT_MODE_MAILBOX_KHR,	// Unlimited frame rate (on Quadro P4000), V-sync (on Radeon WX5100)
			VK_PRESENT_MODE_FIFO_KHR,		// V-sync (on Quadro P4000)
			VK_PRESENT_MODE_FIFO_RELAXED_KHR
		};

		const static uint32_t MAX_FRAMES_IN_FLIGHT = 2;
		const static uint32_t MAX_DESCRIPTOR_SETS_PER_EFFECT = 64; // TODO: this is not a good solution, since scene complexity varies a lot

#if defined(_DEBUG)
		const bool m_enableValidationLayers = true;
#else
		const bool m_enableValidationLayers = false;
#endif

		std::vector<const char*> m_desiredExtensions;
		VkApplicationInfo m_appInfo;

	public:
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_logicalDevice;

		// TODO: find ways to remove these temp members
		std::shared_ptr<DrawingRawSwapchain_Vulkan> t_pSwapchain;
		std::vector<std::shared_ptr<DrawingRawShaderEffect_Vulkan>> t_shaderEffects;
		// Legacy sync ids
		uint32_t t_recordingCmdBufferID = 0; // Later these could be linked with thread id
		uint32_t t_renderFinishedSemaphoreID = -1;
#if defined(ENABLE_TRANSFER_QUEUE_VK)
		uint32_t t_recordingTransCmdBufferID = 0;
#endif

	public:
		DrawingDevice_Vulkan();
		virtual ~DrawingDevice_Vulkan();

		void Initialize() override;
		void Shutdown() override;

		bool CreateVertexFormat(const DrawingVertexFormatDesc& desc, std::shared_ptr<DrawingVertexFormat>& pRes) override;
		bool CreateVertexBuffer(const DrawingVertexBufferDesc& desc, std::shared_ptr<DrawingVertexBuffer>& pRes, std::shared_ptr<DrawingResource> pRefRes, const void* pData = nullptr, uint32_t size = 0) override;
		bool CreateIndexBuffer(const DrawingIndexBufferDesc& desc, std::shared_ptr<DrawingIndexBuffer>& pRes, std::shared_ptr<DrawingResource> pRefRes, const void* pData = nullptr, uint32_t size = 0) override;
		bool CreateTexture(const DrawingTextureDesc& desc, std::shared_ptr<DrawingTexture>& pRes, const void* pData[] = nullptr, uint32_t size[] = nullptr, uint32_t slices = 0) override;
		bool CreateTarget(const DrawingTargetDesc& desc, std::shared_ptr<DrawingTarget>& pRes) override;
		bool CreateDepthBuffer(const DrawingDepthBufferDesc& desc, std::shared_ptr<DrawingDepthBuffer>& pRes) override;

		bool CreateBlendState(const DrawingBlendStateDesc& desc, std::shared_ptr<DrawingBlendState>& pRes) override;
		bool CreateDepthState(const DrawingDepthStateDesc& desc, std::shared_ptr<DrawingDepthState>& pRes) override;
		bool CreateRasterState(const DrawingRasterStateDesc& desc, std::shared_ptr<DrawingRasterState>& pRes) override;
		bool CreateSamplerState(const DrawingSamplerStateDesc& desc, std::shared_ptr<DrawingSamplerState>& pRes) override;

		bool CreateEffectFromFile(const DrawingEffectDesc& desc, std::shared_ptr<DrawingEffect>& pRes) override;
		bool CreateEffectFromString(const std::string& str, const DrawingEffectDesc& desc, std::shared_ptr<DrawingEffect>& pRes) override;
		bool CreateEffectFromBuffer(const void* pData, uint32_t length, const DrawingEffectDesc& desc, std::shared_ptr<DrawingEffect>& pRes) override;
		bool CreateEffectFromShader(const DrawingEffectDesc& desc, std::shared_ptr<DrawingVertexShader> pVSShader, std::shared_ptr<DrawingPixelShader> pPSShader, std::shared_ptr<DrawingEffect>& pRes) override;

		bool CreateVertexShaderFromFile(const DrawingVertexShaderDesc& desc, std::shared_ptr<DrawingVertexShader>& pRes) override;
		bool CreateVertexShaderFromString(const std::string& str, const DrawingVertexShaderDesc& desc, std::shared_ptr<DrawingVertexShader>& pRes) override;
		bool CreateVertexShaderFromBuffer(const void* pData, uint32_t length, const DrawingVertexShaderDesc& desc, std::shared_ptr<DrawingVertexShader>& pRes) override;
		bool CreatePixelShaderFromFile(const DrawingPixelShaderDesc& desc, std::shared_ptr<DrawingPixelShader>& pRes) override;
		bool CreatePixelShaderFromString(const std::string& str, const DrawingPixelShaderDesc& desc, std::shared_ptr<DrawingPixelShader>& pRes) override;
		bool CreatePixelShaderFromBuffer(const void* pData, uint32_t length, const DrawingPixelShaderDesc& desc, std::shared_ptr<DrawingPixelShader>& pRes) override;

		bool CreatePipelineState(const DrawingPipelineStateDesc& desc, const DrawingPipelineState::SubobjectResourceTable& subobjectResources, std::shared_ptr<DrawingPipelineState>& pRes) override;

		void ClearTarget(std::shared_ptr<DrawingTarget> pTarget, const float4& color) override;
		void ClearDepthBuffer(std::shared_ptr<DrawingDepthBuffer> pDepthBuffer, float depth, uint8_t stencil, uint32_t flag) override;

		void SetVertexFormat(std::shared_ptr<DrawingVertexFormat> pFormat) override;
		void SetVertexBuffer(std::shared_ptr<DrawingVertexBuffer> pVB[], uint32_t count) override;
		void SetIndexBuffer(std::shared_ptr<DrawingIndexBuffer> pIB) override;

		void UpdateTexture(std::shared_ptr<DrawingTexture> pTex, const void* pData[], uint32_t size[], uint32_t slices = 0) override;

		void SetBlendState(std::shared_ptr<DrawingBlendState> pBlend, float4 blendFactor, uint32_t sampleMask) override;
		void SetDepthState(std::shared_ptr<DrawingDepthState> pDepth, uint32_t stencilRef) override;
		void SetRasterState(std::shared_ptr<DrawingRasterState> pRaster) override;

		void SetPipelineState(std::shared_ptr<DrawingPipelineState> pPipelineState) override;

		void PushBlendState() override;
		void PopBlendState() override;
		void PushDepthState() override;
		void PopDepthState() override;
		void PushRasterState() override;
		void PopRasterState() override;

		void SetViewport(Box2* vp) override;

		void SetTargets(std::shared_ptr<DrawingTarget> pTarget[], uint32_t maxTargets, std::shared_ptr<DrawingDepthBuffer> pDepthBuffer, std::shared_ptr<DrawingRWBuffer> pRWBuffer[], uint32_t maxRWBuffers) override;

		bool UpdateEffectParameter(std::shared_ptr<DrawingParameter> pParam, std::shared_ptr<DrawingEffect> pEffect) override;
		bool UpdateEffectTexture(std::shared_ptr<DrawingTexture> pTex, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect) override;
		bool UpdateEffectBuffer(std::shared_ptr<DrawingTexBuffer> pBuffer, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect) override;
		bool UpdateEffectSampler(std::shared_ptr<DrawingSamplerState> pSampler, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect) override;
		bool UpdateEffectTexBuffer(std::shared_ptr<DrawingTexBuffer> pTexBuffer, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect) override;
		bool UpdateEffectRWBuffer(std::shared_ptr<DrawingRWBuffer> pRWBuffer, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect) override;
		bool UpdateEffectInputRWBuffer(std::shared_ptr<DrawingRWBuffer> pRWBuffer, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect) override;
		bool UpdateEffectOutputRWBuffer(std::shared_ptr<DrawingRWBuffer> pRWBuffer, std::shared_ptr<std::string> pName, std::shared_ptr<DrawingEffect> pEffect) override;

		void BeginEffect(DrawingContext& dc, std::shared_ptr<DrawingEffect> pEffect) override;
		void EndEffect(DrawingContext& dc, std::shared_ptr<DrawingEffect> pEffect) override;

		bool DrawPrimitive(std::shared_ptr<DrawingPrimitive> pRes) override;
		bool Present(const std::shared_ptr<DrawingTarget> pTarget, uint32_t syncInterval) override;

		void* Map(std::shared_ptr<DrawingResource> pRes, uint32_t subID, EDrawingAccessType flag, uint32_t& rowPitch, uint32_t& slicePitch, uint32_t offset = 0, uint32_t sizeInBytes = 0) override;
		void UnMap(std::shared_ptr<DrawingResource> pRes, uint32_t subID) override;

		void Flush() override;

		uint32_t FormatBytes(EDrawingFormatType type) override;

	public:
		bool CreateInstance();
		bool SetupDebugMessenger();
		bool CreatePresentationSurface();
		bool SelectPhysicalDevice();
		bool CreateLogicalDevice();
		
		std::shared_ptr<DrawingCommandManager_Vulkan> GetCommandManager(EDrawingCommandListType type) const;
		std::shared_ptr<DrawingCommandSyncObejctManager_Vulkan> GetSyncObjectManager() const;
		std::shared_ptr<DrawingUploadAllocator_Vulkan> GetUploadAllocator() const;
		std::shared_ptr<DrawingDescriptorAllocator_Vulkan> GetDescriptorAllocator() const;
#if defined(POOLED_TEXTURE_VK)
		std::shared_ptr<DrawingRawTexturePool_Vulkan> GetTexturePool(uint32_t id);
#endif
#if defined(POOLED_STAGING_BUFFER_VK)
		std::shared_ptr<DrawingRawStagingBufferPool_Vulkan> GetStagingBufferPool() const;
#endif

		const VkPhysicalDeviceLimits& GetDeviceLimits() const;
		const VkFence GetFrameFence() const;

		VkImageView CreateImageView(VkImage& image, VkImageViewType type, VkImageAspectFlags aspect, VkFormat format, uint32_t mipLevels);

	private:
		VkInstance m_instance;
		VkSurfaceKHR m_surface;

		VkPhysicalDeviceProperties m_deviceProperties;
		std::vector<VkExtensionProperties> m_availableExtensions;

		QueueFamilyIndices_Vulkan m_queueFamilyIndices;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;
#if defined(ENABLE_TRANSFER_QUEUE_VK)
		VkQueue m_transferQueue;
#endif

		VkDebugUtilsMessengerEXT m_debugMessenger;

		std::shared_ptr<DrawingUploadAllocator_Vulkan> m_pUploadAllocator;
		std::shared_ptr<DrawingDescriptorAllocator_Vulkan> m_pDescriptorAllocator;
		std::shared_ptr<DrawingCommandSyncObejctManager_Vulkan> m_pSyncObjectManager;
		std::shared_ptr<DrawingCommandManager_Vulkan> m_pDirectCommandManager;
#if defined(ENABLE_TRANSFER_QUEUE_VK)
		std::shared_ptr<DrawingCommandManager_Vulkan> m_pTransferCommandManager;
#endif

#if defined(POOLED_TEXTURE_VK)
		uint32_t m_nextTexturePoolID = 0;
		std::unordered_map<uint32_t, std::shared_ptr<DrawingRawTexturePool_Vulkan>> m_texturePools; // (id, pool)
#endif

#if defined(POOLED_STAGING_BUFFER_VK)
		std::shared_ptr<DrawingRawStagingBufferPool_Vulkan> m_pStagingBufferPool;
#endif

		uint32_t m_currentFrame = 0;
		bool m_isRunning = false;

		// Currently there should be 2 render passes stored in runtime. The first is used for the first pass of a frame, the second is used for subsequent passes
		std::vector<VkRenderPass> m_renderPasses;

		// Legacy sync objects
		std::vector<uint32_t> m_imageAvailableSemaphoreIDs;
		std::vector<uint32_t> m_fenceIDs;

		// TODO: find ways to remove these temp members
		float4 t_clearColor;
		bool t_awaitClearTarget = true;
		bool t_newlyAcquiredImage = false;

	private:
		bool SetupDescriptorAllocator();
		bool SetupCommandManager();
		bool SetupUploadAllocator();

		bool IsDeviceSuitable(const VkPhysicalDevice& device);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availableModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		VkFormat FindDepthFormat();

		bool PrepareRenderPasses(); // This function is dedicated to current device usage (not for general purpose)
		VkRenderPass CreateRenderPass(const std::vector<VkAttachmentDescription>& attachmentDescs, const std::vector<VkSubpassDescription>& subpassDescs, const std::vector<VkSubpassDependency>& dependencies);
		VkSampler CreateTextureSampler(); // TODO: make this function accept parameters
		bool GenerateMipmaps(DrawingRawImage_Vulkan& image);

		// TODO: implement compatible swapchain re-creation functions
		bool RecreateSwapchain();
		void CleanUpSwapchain();

		template<typename T, typename U>
		void* MapResource(std::shared_ptr<DrawingResource> pRes, uint32_t subID);

		template<typename T, typename U>
		void UnMapResource(std::shared_ptr<DrawingResource> pRes, uint32_t aSubID);
	};

	template<>
	static std::shared_ptr<DrawingDevice> CreateNativeDevice<EConfigurationDeviceType::eDevice_Vulkan>()
	{
#if defined(_DEBUG)
		std::cout << "Creating Vulkan device..." << std::endl;
#endif

		std::shared_ptr<DrawingDevice_Vulkan> pDevice = std::make_shared<DrawingDevice_Vulkan>();
		pDevice->m_desiredExtensions = GetRequiredExtensions_Vulkan(pDevice->m_enableValidationLayers);
		
		pDevice->CreateInstance();
		pDevice->SetupDebugMessenger();
		pDevice->CreatePresentationSurface();
		pDevice->SelectPhysicalDevice();
		pDevice->CreateLogicalDevice();

		return pDevice;
	}

	struct WindowParameters_Vulkan
	{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		HINSTANCE HInstance;
		HWND HWnd;

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		Display* Dpy;
		Window Window;

#elif defined(VK_USE_PLATFORM_XCB_KHR)
		xcb_connection_t* Connection;
		xcb_window_t Window;

#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
		//...

#elif defined(VK_USE_PLATFORM_IOS_MVK)
		//...

#elif defined(VK_USE_PLATFORM_MACOS_MVK)
		//...

#endif
	};

	static uint32_t GetTypeElementSize_Vulkan(const spirv_cross::SPIRType& type)
	{
		switch (type.basetype)
		{
		case spirv_cross::SPIRType::BaseType::Boolean:
		case spirv_cross::SPIRType::BaseType::SByte:
		case spirv_cross::SPIRType::BaseType::UByte:
			return 1U;
		case spirv_cross::SPIRType::BaseType::Short:
		case spirv_cross::SPIRType::BaseType::UShort:
		case spirv_cross::SPIRType::BaseType::Half:
			return 2U;
		case spirv_cross::SPIRType::BaseType::Int:
		case spirv_cross::SPIRType::BaseType::UInt:
		case spirv_cross::SPIRType::BaseType::Float:
			return 4U;
		case spirv_cross::SPIRType::BaseType::Int64:
		case spirv_cross::SPIRType::BaseType::UInt64:
		case spirv_cross::SPIRType::BaseType::Double:
			return 8U;
		default:
			throw std::runtime_error("Unsupported Vulkan shader struct element type.");
			break;
		}
		return 0U;
	}

	static EBasicType BasicTypeConvert_Vulkan(const spirv_cross::SPIRType& type)
	{
		switch (type.basetype)
		{
		case spirv_cross::SPIRType::BaseType::Boolean:
			return eBasic_Bool;
		case spirv_cross::SPIRType::BaseType::Int:
			return eBasic_Int32;
		case spirv_cross::SPIRType::BaseType::UInt:
			return eBasic_UInt32;
		case spirv_cross::SPIRType::BaseType::Float:
			return eBasic_FP32;
		case spirv_cross::SPIRType::BaseType::Double:
			return eBasic_FP64;

		case spirv_cross::SPIRType::BaseType::SByte:
		case spirv_cross::SPIRType::BaseType::UByte:
		case spirv_cross::SPIRType::BaseType::Short:
		case spirv_cross::SPIRType::BaseType::UShort:
		case spirv_cross::SPIRType::BaseType::Half:
		case spirv_cross::SPIRType::BaseType::Int64:
		case spirv_cross::SPIRType::BaseType::UInt64:
		default:
			throw std::runtime_error("Unhandled Vulkan shader param basic type.");
			return eBasic_FP32;
		}
	}

	static uint32_t GetParamType_Vulkan(const spirv_cross::SPIRType& type, uint32_t& size)
	{
		uint32_t paramType = (uint32_t)EParam_Invalid;

		switch (type.basetype)
		{
		case spirv_cross::SPIRType::BaseType::Image:
		case spirv_cross::SPIRType::BaseType::SampledImage: // Alert: not sure if this is combined image sampler
		{
			paramType = COMPOSE_TYPE(eObject_Texture, eDataSet_Object, eBasic_FP32, 0, 0, 0);
			break;
		}
		case spirv_cross::SPIRType::BaseType::Sampler:
		{
			paramType = COMPOSE_TYPE(eObject_Sampler, eDataSet_Object, eBasic_FP32, 0, 0, 0);
			break;
		}
		case spirv_cross::SPIRType::BaseType::Boolean:
		case spirv_cross::SPIRType::BaseType::SByte:
		case spirv_cross::SPIRType::BaseType::UByte:
		case spirv_cross::SPIRType::BaseType::Short:
		case spirv_cross::SPIRType::BaseType::UShort:
		case spirv_cross::SPIRType::BaseType::Half:
		case spirv_cross::SPIRType::BaseType::Int:
		case spirv_cross::SPIRType::BaseType::UInt:
		case spirv_cross::SPIRType::BaseType::Float:
		case spirv_cross::SPIRType::BaseType::Int64:
		case spirv_cross::SPIRType::BaseType::UInt64:
		case spirv_cross::SPIRType::BaseType::Double:
		{
			uint32_t rowSize = 0;
			uint32_t colSize = 0;
			uint32_t arraySize = 0;
			uint32_t dataSetType = eDataSet_Scalar;
			uint32_t basicType = BasicTypeConvert_Vulkan(type);

			if (!type.array.empty())
			{
				arraySize = static_cast<uint32_t>(type.array.size());
			}

			if (type.columns > 1)
			{
				dataSetType = eDataSet_Matrix;
				colSize = type.columns;
				rowSize = size / (colSize * GetTypeElementSize_Vulkan(type));		
			}
			else if (type.vecsize > 1)// Alert: this may also be incorrect
			{
				dataSetType = eDataSet_Vector;
				rowSize = type.vecsize;
			}

			paramType = COMPOSE_TYPE(eObject_Value, dataSetType, basicType, arraySize, colSize, rowSize);
			break;
		}
		default:
		{
			throw std::runtime_error("Unsupported Vulkan shader param type.");
			return (uint32_t)EParam_Invalid;
		}
		}

		return paramType;
	}
}
