#pragma once

#include "VkHandle.h"

namespace ZHU
{
	class VkRenderDevice : public VkHandleBase
		                 , public inherit_shared_from_this<VkHandleBase, VkRenderDevice>
	{
	public:
		class SharedPtr : public std::shared_ptr<VkRenderDevice>
		{
		public:
			SharedPtr() = default;
			SharedPtr(VkRenderDevice* data) : std::shared_ptr<VkRenderDevice>(data) {}

			static SharedPtr Create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface)
			{
				return SharedPtr(new VkRenderDevice(instance, physicalDevice, device, surface));
			}

			operator VkInstance() const { return get()->mInstance; }
			operator VkPhysicalDevice() const { return get()->mPhysicalDevice; }
			operator VkDevice() const { return get()->mLogicalDevice; }
			operator VkSurfaceKHR() const { return get()->mSurface; }
		};
		friend SharedPtr;

		virtual ~VkRenderDevice();

	protected:
		VkRenderDevice(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface)
			: mInstance(instance)
			, mPhysicalDevice(physicalDevice)
			, mLogicalDevice(device)
			, mSurface(surface) 
		{}

	private:
		VkInstance mInstance;
		VkPhysicalDevice mPhysicalDevice;
		VkDevice mLogicalDevice;
		VkSurfaceKHR mSurface;
	};
}