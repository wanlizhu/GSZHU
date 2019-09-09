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

	template<typename HANDLE>
	class VkHandle : public VkHandleBase
		           , public inherit_shared_from_this<VkHandleBase, VkHandle<HANDLE>>
	{
	public:
		class SharedPtr : public std::shared_ptr<VkHandle<HANDLE>>
		{
		public:
			SharedPtr = default;
			SharedPtr(VkHandle<HANDLE>* handle) : std::shared_ptr<VkHandle<HANDLE>>(handle) {}

			static SharedPtr Create(HANDLE handle) { return SharedPtr(new VkHandle<HANDLE>(handle)); }
			operator HANDLE() const { return get()->mHandle; }
		};
		friend class SharedPtr;

		virtual ~VkHandle() { static_assert(false, "VkHandle missing destructor specialization"); }

	private:
		VkHandle(const HANDLE& handle) : mHandle(handle) {}

	private:
		HANDLE mHandle;
	};

	template<> VkHandle<VkSwapchainKHR>::~VkHandle();
	template<> VkHandle<VkCommandPool>::~VkHandle();
	template<> VkHandle<VkSemaphore>::~VkHandle();
	template<> VkHandle<VkSampler>::~VkHandle();
	template<> VkHandle<VkDescriptorSetLayout>::~VkHandle();
	template<> VkHandle<VkPipeline>::~VkHandle();
	template<> VkHandle<VkShaderModule>::~VkHandle();
	template<> VkHandle<VkPipelineLayout>::~VkHandle();
	template<> VkHandle<VkDescriptorPool>::~VkHandle();
	template<> VkHandle<VkQueryPool>::~VkHandle();

}