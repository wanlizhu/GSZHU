#pragma once

#include <type_traits>
#include <vulkan/vulkan.h>
#include "LowLevel/Utils.h"

namespace ZHU
{
	class VkHandleBase : public std::enable_shared_from_this<VkHandleBase>
	{
	public:
		using SharedPtr = std::shared_ptr<VkHandleBase>;
		virtual ~VkHandleBase() = default;
	};

	template<typename _NativeHandle>
	class VkHandle : public VkHandleBase
		           , public inherit_shared_from_this<VkHandleBase, VkHandle<_NativeHandle>>
	{
	public:
		using NativeHandle = _NativeHandle;
		class SharedPtr : public std::shared_ptr<VkHandle<NativeHandle>>
		{
		public:
			SharedPtr = default;
			SharedPtr(VkHandle<NativeHandle>* handle) 
				: std::shared_ptr<VkHandle<NativeHandle>>(handle)
			{}

			static SharedPtr Create(NativeHandle handle)
			{
				return SharedPtr(new VkHandle<NativeHandle>(handle));
			}
			operator NativeHandle() const 
			{
				return get()->mNativeHandle;
			}
		};
		friend class SharedPtr;

		virtual ~VkHandle() { static_assert(false, "VkHandle missing destructor specialization"); }

	protected:
		VkHandle(const NativeHandle& handle) 
			: mNativeHandle(handle)
		{}

	protected:
		NativeHandle mNativeHandle;
	};

	// Force template instantiation
	template<> VkHandle<VkSwapchainKHR>::~VkHandle();
	template<> VkHandle<VkCommandBuffer>::~VkHandle();
	template<> VkHandle<VkCommandPool>::~VkHandle();
	template<> VkHandle<VkQueue>::~VkHandle();
	template<> VkHandle<VkSemaphore>::~VkHandle();
	template<> VkHandle<VkSampler>::~VkHandle(); 
	template<> VkHandle<VkDescriptorSet>::~VkHandle();
	template<> VkHandle<VkDescriptorSetLayout>::~VkHandle();
	template<> VkHandle<VkPipeline>::~VkHandle();
	template<> VkHandle<VkShaderModule>::~VkHandle();
	template<> VkHandle<VkPipelineLayout>::~VkHandle();
	template<> VkHandle<VkDescriptorPool>::~VkHandle();
	template<> VkHandle<VkQueryPool>::~VkHandle();

}