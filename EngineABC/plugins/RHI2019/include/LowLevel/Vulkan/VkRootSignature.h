#pragma once

#include "VkHandle.h"
#include <vector>

namespace ZHU
{
	class VkRootSignature : public VkHandle<VkPipelineLayout>
		                  , public inherit_shared_from_this<VkHandle<VkPipelineLayout>, VkRootSignature>
	{
	public:
		class SharedPtr : public std::shared_ptr<VkRootSignature>
		{
		public:
			SharedPtr() = default;
			SharedPtr(VkRootSignature* handle) 
				: std::shared_ptr<VkRootSignature>(handle)
			{}
			
			static SharedPtr Create(VkPipelineLayout layout, 
									const std::vector<VkDescriptorSetLayout>& sets)
			{
				return SharedPtr(new VkRootSignature(layout, sets));
			}
			operator VkPipelineLayout() const 
			{
				return get()->mNativeHandle;
			}
		};
		friend class SharedPtr;

		virtual ~VkRootSignature();

	private:
		VkRootSignature(VkPipelineLayout layout, 
						const std::vector<VkDescriptorSetLayout>& sets) 
			: VkHandle<VkPipelineLayout>(layout)
			, mDescSetLayouts(sets)
		{}

	private:
		std::vector<VkDescriptorSetLayout> mDescSetLayouts;
	};
}