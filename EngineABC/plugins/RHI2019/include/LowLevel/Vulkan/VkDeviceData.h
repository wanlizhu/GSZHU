#pragma once

#include "VkHandle.h"

namespace ZHU
{
	class VkDeviceData : public VkHandleBase
		               , public inherit_shared_from_this<VkHandleBase, VkDeviceData>
	{
	public:
		class SharedPtr : public std::shared_ptr<VkDeviceData>
		{
		public:
			SharedPtr() = default;
			SharedPtr(VkDeviceData* data) : std::shared_ptr<VkDeviceData>(data) {}

			static SharedPtr Create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface)
			{
				return SharedPtr(new VkDeviceData(instance, physicalDevice, device, surface));
			}

			operator VkInstance() const { return get()->mInstance; }
			operator VkPhysicalDevice() const { return get()->mPhysicalDevice; }
			operator VkDevice() const { return get()->mLogicalDevice; }
			operator VkSurfaceKHR() const { return get()->mSurface; }
		};
		friend SharedPtr;

		virtual ~VkDeviceData();

	private:
		VkDeviceData(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface)
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