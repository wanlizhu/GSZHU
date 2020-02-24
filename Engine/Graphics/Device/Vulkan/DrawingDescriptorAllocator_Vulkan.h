#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

namespace Engine
{
	enum EDescriptorInfoType_Vulkan
	{
		eDescriptorInfo_Buffer = 0,
		eDescriptorInfo_Image,
		eDescriptorInfo_TexelBuffer,
		eDescriptorInfo_TYPE_COUNT
	};

	typedef struct _DescriptorUpdateInfo
	{
		VkDescriptorSet						TargetDescriptorSet;
		uint32_t							TargetDescriptorBinding;
		uint32_t							TargetArrayElement;
		VkDescriptorType					TargetDescriptorType;
		EDescriptorInfoType_Vulkan			InfoType;
		std::vector<VkDescriptorBufferInfo>	BufferInfos;
		std::vector<VkDescriptorImageInfo>	ImageInfos;
		std::vector<VkBufferView>			TexelBufferViews;
		bool								HasContent;
	} DescriptorUpdateInfo_Vulkan;

	typedef struct _CopyDescriptorInfo
	{
		VkDescriptorSet	TargetDescriptorSet;
		uint32_t		TargetDescriptorBinding;
		uint32_t		TargetArrayElement;
		VkDescriptorSet	SourceDescriptorSet;
		uint32_t		SourceDescriptorBinding;
		uint32_t		SourceArrayElement;
		uint32_t		DescriptorCount;
	} CopyDescriptorInfo_Vulkan;

	class DrawingDevice_Vulkan;
	class DrawingDescriptorAllocator_Vulkan
	{
	public:
		DrawingDescriptorAllocator_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice);
		virtual ~DrawingDescriptorAllocator_Vulkan();

		void Destroy();
		void DestroyPools();

		uint32_t CreateDescriptorPool(uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes);
		bool AllocateDescriptorSets(uint32_t descriptorPoolID, const std::vector<VkDescriptorSetLayout>& layouts, std::vector<VkDescriptorSet>& outputSets, bool clearPreviousSets = false);
		bool UpdateDescriptorSets(const std::vector<DescriptorUpdateInfo_Vulkan>& updateInfo);

	private:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		std::vector<VkDescriptorPool> m_descriptorPools;
	};

	class DrawingDescriptorSetLayout_Vulkan
	{
	public:
		DrawingDescriptorSetLayout_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice);
		DrawingDescriptorSetLayout_Vulkan(const std::shared_ptr<DrawingDevice_Vulkan> pDevice, const std::vector<VkDescriptorSetLayoutBinding>& bindings);
		virtual ~DrawingDescriptorSetLayout_Vulkan();

		const VkDescriptorSetLayout GetDescriptorSetLayout() const;
		uint32_t GetDescriptorSetBitMask() const;
		uint32_t GetNumDescriptors(uint32_t index) const;

	private:
		std::shared_ptr<DrawingDevice_Vulkan> m_pDevice;
		VkDescriptorSetLayout m_descriptorSetLayout;
		
		// These members have not been used
		uint32_t m_numDescriptorsPerSet[32];
		uint32_t m_descriptorSetBitMask;
	};
}