#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

#include "DrawingConstants.h"
#include "DrawingRawResource.h"

namespace Engine
{
	inline VkShaderModule VK_LoadSPIRVShader(const std::string& filename, const VkDevice& device, bool preserveCode, std::vector<char>& rawCode)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary | std::ios::in);

		if (!file.is_open())
		{
			throw std::runtime_error("Failed to open: " + filename);
			return VK_NULL_HANDLE;
		}

		size_t fileSize = (size_t)file.tellg();
		assert(fileSize > 0);
		std::vector<char> buffer(fileSize);

		file.seekg(0, std::ios::beg);
		file.read(buffer.data(), fileSize);
		file.close();

		if (buffer.size() > 0)
		{
			VkShaderModuleCreateInfo moduleCreateInfo = {};
			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.codeSize = fileSize;
			moduleCreateInfo.pCode = reinterpret_cast<uint32_t*>(buffer.data());

			if (preserveCode)
			{
				rawCode.resize(fileSize);
				memcpy(rawCode.data(), buffer.data(), fileSize);
			}

			VkShaderModule shaderModule = VK_NULL_HANDLE;
			VkResult result = vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &shaderModule);

			buffer.clear();
			buffer.resize(0);

			if (result != VK_SUCCESS)
			{
				throw std::runtime_error("Could not create shader from file: " + filename);
				return VK_NULL_HANDLE;
			}

			return shaderModule;
		}
		else
		{
			throw std::runtime_error("Failed to read: " + filename);
			return VK_NULL_HANDLE;
		}
	}

	inline VkShaderModule VK_LoadSPIRVShader(const std::string& filename, const VkDevice& device)
	{
		std::vector<char> straw;
		return VK_LoadSPIRVShader(filename, device, false, straw);
	}

	inline bool VK_CheckResult(const VkResult func, const std::string msg = "Vulkan function call")
	{
		if (func != VK_SUCCESS)
		{
			throw std::runtime_error("Error in: " + msg);
			return false;
		}
		// Could do extra error message printing here
		return true;
	}

	// Alert: this function could cause error if VkImageType changes in the future
	inline VkImageType VK_DetermineImageType(const VkExtent3D& extent)
	{
		int dimensions = -1;
		extent.width  > 1 ? dimensions += 1 : dimensions += 0;
		extent.height > 1 ? dimensions += 1 : dimensions += 0;
		extent.depth  > 1 ? dimensions += 1 : dimensions += 0;

		dimensions = dimensions > 0 ? dimensions : 0;

		return static_cast<VkImageType>(dimensions);
	}

	inline bool VK_HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	inline VkFormat VK_FormatEnum(EDrawingFormatType format)
	{
		switch (format)
		{
		case eFormat_Unknown:
			return VK_FORMAT_UNDEFINED;
		case eFormat_D24S8:
			return VK_FORMAT_D24_UNORM_S8_UINT;
		case eFormat_D24X8:
			return VK_FORMAT_D24_UNORM_S8_UINT;
		case eFormat_D32_FLOAT:
			return VK_FORMAT_D32_SFLOAT;
		case eFormat_R8_UNORM:
			return VK_FORMAT_R8_UNORM;
		case eFormat_R8_SNORM:
			return VK_FORMAT_R8_SNORM;
		case eFormat_R8_UINT:
			return VK_FORMAT_R8_UINT;
		case eFormat_R8_SINT:
			return VK_FORMAT_R8_SINT;
		case eFormat_R32_FLOAT:
			return VK_FORMAT_R32_SFLOAT;
		case eFormat_R32_UINT:
			return VK_FORMAT_R32_UINT;
		case eFormat_R32_SINT:
			return VK_FORMAT_R32_SINT;
		case eFormat_R32G32_FLOAT:
			return VK_FORMAT_R32G32_SFLOAT;
		case eFormat_R32G32_UINT:
			return VK_FORMAT_R32G32_UINT;
		case eFormat_R32G32_SINT:
			return VK_FORMAT_R32G32_SINT;
		case eFormat_R8G8B8A8_UNORM:
			return VK_FORMAT_R8G8B8A8_UNORM;
		case eFormat_R8G8B8A8_SNORM:
			return VK_FORMAT_R8G8B8A8_SNORM;
		case eFormat_R8G8B8A8_UINT:
			return VK_FORMAT_R8G8B8A8_UINT;
		case eFormat_R8G8B8A8_SINT:
			return VK_FORMAT_R8G8B8A8_SINT;
		case eFormat_R32G32B32_FLOAT:
			return VK_FORMAT_R32G32B32_SFLOAT;
		case eFormat_R32G32B32_UINT:
			return VK_FORMAT_R32G32B32_UINT;
		case eFormat_R32G32B32_SINT:
			return VK_FORMAT_R32G32B32_SINT;
		case eFormat_R32G32B32A32_FLOAT:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case eFormat_R32G32B32A32_UINT:
			return VK_FORMAT_R32G32B32A32_UINT;
		case eFormat_R32G32B32A32_SINT:
			return VK_FORMAT_R32G32B32A32_SINT;
		}

		throw std::runtime_error("Unsupported Vulkan format conversion.");
		return VK_FORMAT_UNDEFINED;
	}

	inline uint32_t VK_FormatBytes(VkFormat format)
	{
		switch (format)
		{
		case VK_FORMAT_R8_UNORM:
		case VK_FORMAT_R8_SNORM:
		case VK_FORMAT_R8_UINT:
		case VK_FORMAT_R8_SINT:
			return 1U;
		case VK_FORMAT_D24_UNORM_S8_UINT:
		case VK_FORMAT_D32_SFLOAT:
		case VK_FORMAT_R32_SFLOAT:
		case VK_FORMAT_R32_UINT:
		case VK_FORMAT_R32_SINT:
		case VK_FORMAT_R8G8B8A8_UNORM:
		case VK_FORMAT_R8G8B8A8_SNORM:
		case VK_FORMAT_R8G8B8A8_UINT:
		case VK_FORMAT_R8G8B8A8_SINT:
			return 4U;
		case VK_FORMAT_R32G32_SFLOAT:
		case VK_FORMAT_R32G32_UINT:
		case VK_FORMAT_R32G32_SINT:
			return 8U;
		case VK_FORMAT_R32G32B32_SFLOAT:
		case VK_FORMAT_R32G32B32_UINT:
		case VK_FORMAT_R32G32B32_SINT:
			return 12U;
		case VK_FORMAT_R32G32B32A32_SFLOAT:
		case VK_FORMAT_R32G32B32A32_UINT:
		case VK_FORMAT_R32G32B32A32_SINT:
			return 16U;
		}

		throw std::runtime_error("Unhandled Vulkan format size in bytes.");
		return 0U;
	}

	inline uint32_t VK_FormatBytes(EDrawingFormatType format)
	{
		switch (format)
		{
		case eFormat_D24S8:
		case eFormat_D24X8:
		case eFormat_D32_FLOAT:
		case eFormat_R32_UINT:
		case eFormat_R32_SINT:
		case eFormat_R32_FLOAT:
			return 4U;
		case eFormat_R32G32_UINT:
		case eFormat_R32G32_SINT:
		case eFormat_R32G32_FLOAT:
			return 8U;
		case eFormat_R32G32B32_UINT:
		case eFormat_R32G32B32_SINT:
		case eFormat_R32G32B32_FLOAT:
			return 12U;
		case eFormat_R32G32B32A32_UINT:
		case eFormat_R32G32B32A32_SINT:
		case eFormat_R32G32B32A32_FLOAT:
			return 16U;
		}

		return 0U;
	}

	inline VkPrimitiveTopology VK_PrimitiveTopologyEnum(EDrawingPrimitiveType prim)
	{
		switch (prim)
		{
		case ePrimitive_Undefined:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST; // Comment: there is no undefined type in Vulkan
		case ePrimitive_PointList:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		case ePrimitive_LineList:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case ePrimitive_LineStrip:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		case ePrimitive_LineListAdj:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
		case ePrimitive_LineStripAdj:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
		case ePrimitive_TriangleList:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case ePrimitive_TriangleStrip:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		case ePrimitive_TriangleListAdj:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
		case ePrimitive_TriangleStripAdj:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
		}

		throw std::runtime_error("Unsupported Vulkan primitive topology conversion.");
		return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
	}

	inline void VK_PrintPhysicalDeviceInfo(const VkPhysicalDeviceProperties& properties)
	{
		std::cout << "Device: " << properties.deviceName << std::endl;
		std::cout << "Device ID: " << properties.deviceID << std::endl;

		std::string deviceType = "";
		switch (properties.deviceType)
		{
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			deviceType = "OTHER";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			deviceType = "INTEGRATED";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			deviceType = "DISCRETE";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			deviceType = "VIRTUAL";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			deviceType = "CPU";
			break;
		}

		std::cout << "Device Type: " << deviceType << std::endl;
		std::cout << "API Version: " << properties.apiVersion << std::endl;
		std::cout << "Driver Version: " << properties.driverVersion << std::endl;
		std::cout << "Vendor: " << properties.vendorID << std::endl;

		// Can print extra limits and sparse properties here
		std::cout << "Max PushConstants Size: " << properties.limits.maxPushConstantsSize << std::endl;
	}

	inline void VK_PrintPhysicalDeviceInfo(const VkPhysicalDevice& device)
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);
		VK_PrintPhysicalDeviceInfo(properties);
	}

	inline VkShaderStageFlags VK_DetermineShaderStage(DrawingRawShader::DrawingRawShaderType shaderType)
	{
		switch (shaderType)
		{
		case DrawingRawShader::RawShader_VS:
			return VK_SHADER_STAGE_VERTEX_BIT;
			break;
		case DrawingRawShader::RawShader_PS:
			return VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
		case DrawingRawShader::RawShader_CS:
			return VK_SHADER_STAGE_COMPUTE_BIT;
			break;
		}

		throw std::runtime_error("Unhandled Vulkan device shader stage.");
		return VK_SHADER_STAGE_ALL;
	}

	inline VkPipelineStageFlags VK_DetermineShaderPipelineStage(DrawingRawShader::DrawingRawShaderType shaderType)
	{
		switch (shaderType)
		{
		case DrawingRawShader::RawShader_VS:
			return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
			break;
		case DrawingRawShader::RawShader_PS:
			return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			break;
		case DrawingRawShader::RawShader_CS:
			return VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
			break;
		}

		throw std::runtime_error("Unhandled Vulkan device pipeline shader stage.");
		return VK_SHADER_STAGE_ALL;
	}

	inline VkSampleCountFlagBits VK_MultisampleCount(uint32_t sampleCount)
	{
		if (sampleCount <= 1)
			return VK_SAMPLE_COUNT_1_BIT;

		else if (sampleCount <= 2)
			return VK_SAMPLE_COUNT_2_BIT;

		else if (sampleCount <= 4)
			return VK_SAMPLE_COUNT_4_BIT;

		else if (sampleCount <= 8)
			return VK_SAMPLE_COUNT_8_BIT;

		else if (sampleCount <= 16)
			return VK_SAMPLE_COUNT_16_BIT;

		else if (sampleCount <= 32)
			return VK_SAMPLE_COUNT_32_BIT;

		else
			return VK_SAMPLE_COUNT_64_BIT;
	}

	inline VkVertexInputRate VK_VertexInputRate(uint32_t instanceStepRate)
	{
		return instanceStepRate == 0 ? VK_VERTEX_INPUT_RATE_VERTEX : VK_VERTEX_INPUT_RATE_INSTANCE;
	}

	inline void VK_GetAccessAndStageFromLayout(const VkImageLayout layout, VkAccessFlags& accessMask, VkPipelineStageFlags& pipelineStage, DrawingRawShader::DrawingRawShaderType shaderReadStage = DrawingRawShader::RawShader_VS) // Alert: For CAD demo, the default shader read stage is VS
	{
		switch (layout)
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			accessMask = 0;
			pipelineStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			return;

		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			accessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			pipelineStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			return;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			accessMask = VK_ACCESS_SHADER_READ_BIT;
			pipelineStage = VK_DetermineShaderPipelineStage(shaderReadStage);
			return;

		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
			accessMask = 0;
			pipelineStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			return;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			accessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			pipelineStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			// The depth buffer will be read from to perform depth tests to see
			// if a fragment is visible, and will be written to when a new 
			// fragment is drawn. The reading happens in the 
			// VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT stage and the writing
			// in the VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT.
			return;

		default:
			throw std::runtime_error("Unhandled image layout access & stage query.");
			break;
		}
	}
}