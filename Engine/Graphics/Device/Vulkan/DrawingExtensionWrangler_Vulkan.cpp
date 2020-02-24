#include "DrawingExtensionWrangler_Vulkan.h"
#include <iostream>
#include <set>

using namespace Engine;

bool Engine::CheckValidationLayerSupport_Vulkan(const std::vector<const char*>& validationLayers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	bool layerFound = false;
	for (const char* layerName : validationLayers)
	{
		layerFound = false;
		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}

	}

	return true;
}

// Callback function taken from https://vulkan-tutorial.com/code/16_swap_chain_recreation.cpp
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT Engine::GetDebugUtilsMessengerCreateInfo_Vulkan()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = DebugCallback;

	return createInfo;
}

VkResult Engine::CreateDebugUtilsMessengerEXT_Vulkan(const VkInstance& instance, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, &GetDebugUtilsMessengerCreateInfo_Vulkan(), pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void Engine::DestroyDebugUtilMessengerEXT_Vulkan(const VkInstance& instance, VkDebugUtilsMessengerEXT* pDebugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, *pDebugMessenger, pAllocator);
	}
}

bool Engine::CheckAvailableInstanceExtensions_Vulkan(std::vector<VkExtensionProperties>& availableExtensions)
{
	uint32_t extensionsCount = 0;
	VkResult result = VK_SUCCESS;

	result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

	if ((result != VK_SUCCESS) || extensionsCount == 0)
	{
		throw std::runtime_error("Could not get the number of instance extensions.");
		return false;
	}

	availableExtensions.resize(extensionsCount);
	result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, availableExtensions.data());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Could not enumerate instance extensions.");
		return false;
	}

	return true;
}

bool Engine::IsExtensionSupported_Vulkan(const std::vector<VkExtensionProperties>& availableExtensions, const char desiredExtension[VK_MAX_EXTENSION_NAME_SIZE])
{
	for (auto& extension : availableExtensions)
	{
		if (strcmp(desiredExtension, extension.extensionName) == 0)
		{
			return true;
		}
	}

	return false;
}

bool Engine::CheckDeviceExtensionsSupport_Vulkan(const VkPhysicalDevice& device, const std::vector<const char*>& deviceExtensions)
{
	uint32_t extensionCount = 0;

	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

std::vector<const char*> Engine::GetRequiredExtensions_Vulkan(bool enableValidationLayers)
{
	std::vector<const char*> extensions;

	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#if defined(VK_USE_PLATFORM_WIN32_KHR)
	extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);

#elif defined(VK_USE_PLATFORM_XCB_KHR)
	extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);

#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
	extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);

#elif defined(VK_USE_PLATFORM_IOS_MVK)
	extensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);

#elif defined(VK_USE_PLATFORM_MACOS_MVK)
	extensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);

#endif

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

QueueFamilyIndices_Vulkan Engine::FindQueueFamilies_Vulkan(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
{
	QueueFamilyIndices_Vulkan indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	for (uint32_t index = 0; index < static_cast<uint32_t>(queueFamilies.size()); ++index)
	{
		if (queueFamilies[index].queueCount > 0 && queueFamilies[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = index;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &presentSupport);

		if (queueFamilies[index].queueCount > 0 && presentSupport && !indices.presentFamily.has_value())
		{
			indices.presentFamily = index;
		}

#if defined(ENABLE_TRANSFER_QUEUE_VK)
		if (queueFamilies[index].queueCount > 0 && (queueFamilies[index].queueFlags & VK_QUEUE_TRANSFER_BIT) 
			&& !(queueFamilies[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(queueFamilies[index].queueFlags & VK_QUEUE_COMPUTE_BIT))
		{
			indices.transferFamily = index;
		}
#endif

		if (indices.isComplete())
		{
			break;
		}
	}

	if (!indices.isComplete())
	{
		throw std::runtime_error("Vulkan: Could not find required queues.");
	}

	return indices;
}

SwapchainSupportDetails_Vulkan Engine::QuerySwapchainSupport_Vulkan(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
{
	SwapchainSupportDetails_Vulkan details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount > 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount > 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

VkFormat Engine::FindSupportedFormat_Vulkan(const VkPhysicalDevice& physDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(physDevice, format, &properties);

		if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throw std::runtime_error("Failed to find supported format.");
	return VK_FORMAT_UNDEFINED;
}

bool Engine::IsLinearBlittingSupported_Vulkan(const VkPhysicalDevice& physDevice, VkFormat imageFormat)
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(physDevice, imageFormat, &formatProperties);

	return (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);
}