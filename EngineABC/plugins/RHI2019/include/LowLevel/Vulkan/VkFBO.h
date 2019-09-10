#pragma once

#include "VkHandle.h"

namespace ZHU
{
	class VkFBO : public VkHandleBase
		        , public inherit_shared_from_this<VkHandleBase, VkFBO>
	{
	public:
		class SharedPtr : public std::shared_ptr<VkFBO>
		{
		public:
			SharedPtr() = default;
			SharedPtr(VkFBO* fbo) : std::shared_ptr<VkFBO>(fbo) {}

			static SharedPtr create(VkRenderPass renderPass, VkFramebuffer fbo) { return SharedPtr(new VkFBO(renderPass, fbo)); }
			operator VkFramebuffer() const { return get()->mVkFramebuffer; }
			operator VkRenderPass() const { return get()->mVkRenderPass; }
		};
		friend SharedPtr;

		virtual ~VkFBO();

		operator VkRenderPass() const { return mVkRenderPass; }
		operator VkFramebuffer() const { return mVkFramebuffer; }
	private:
		VkFBO(VkRenderPass renderPass, VkFramebuffer fbo)
			: mVkRenderPass(renderPass)
			, mVkFramebuffer(fbo)
		{}

	private:
		VkRenderPass mVkRenderPass = VK_NULL_HANDLE;
		VkFramebuffer mVkFramebuffer = VK_NULL_HANDLE;
	};
}