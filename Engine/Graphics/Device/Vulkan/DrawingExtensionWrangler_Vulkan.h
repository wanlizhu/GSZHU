#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

#include "PerformanceTestParameters_Vulkan.h"

namespace Engine
{
	struct QueueFamilyIndices_Vulkan
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
#if defined(ENABLE_TRANSFER_QUEUE_VK)
		std::optional<uint32_t> transferFamily;
#endif

		bool isComplete()
		{
#if defined(ENABLE_TRANSFER_QUEUE_VK)
			return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
#else
			return graphicsFamily.has_value() && presentFamily.has_value();
#endif
		}
	};

	struct SwapchainSupportDetails_Vulkan
	{
		VkSurfaceCapabilitiesKHR		capabilities = {};
		std::vector<VkSurfaceFormatKHR>	formats;
		std::vector<VkPresentModeKHR>	presentModes;
	};

	// Debug
	bool CheckValidationLayerSupport_Vulkan(const std::vector<const char*>& validationLayers);
	VkDebugUtilsMessengerCreateInfoEXT GetDebugUtilsMessengerCreateInfo_Vulkan();
	VkResult CreateDebugUtilsMessengerEXT_Vulkan(const VkInstance& instance, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilMessengerEXT_Vulkan(const VkInstance& instance, VkDebugUtilsMessengerEXT* pDebugMessenger, const VkAllocationCallbacks* pAllocator);

	// Extension
	bool CheckAvailableInstanceExtensions_Vulkan(std::vector<VkExtensionProperties>& availableExtensions);
	bool IsExtensionSupported_Vulkan(const std::vector<VkExtensionProperties>& availableExtensions, const char desiredExtension[VK_MAX_EXTENSION_NAME_SIZE]);
	bool CheckDeviceExtensionsSupport_Vulkan(const VkPhysicalDevice& device, const std::vector<const char*>& deviceExtensions);
	std::vector<const char*> GetRequiredExtensions_Vulkan(bool enableValidationLayers = false);

	// Queue
	QueueFamilyIndices_Vulkan FindQueueFamilies_Vulkan(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
	
	// Swapchain
	SwapchainSupportDetails_Vulkan QuerySwapchainSupport_Vulkan(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

	// Format
	VkFormat FindSupportedFormat_Vulkan(const VkPhysicalDevice& physDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	bool IsLinearBlittingSupported_Vulkan(const VkPhysicalDevice& physDevice, VkFormat imageFormat);
}