#include "vulkan_common.h"
#include "dejavu/core/strings.h"
#include "dejavu/core/filesystem.h"
#include <fstream>
#include <sstream>

namespace djv
{
    std::string to_string(VkResult error)
    {
        switch (error)
		{
#define STR(r) case r: return #r
			STR(VK_NOT_READY);
			STR(VK_TIMEOUT);
			STR(VK_EVENT_SET);
			STR(VK_EVENT_RESET);
			STR(VK_INCOMPLETE);
			STR(VK_ERROR_OUT_OF_HOST_MEMORY);
			STR(VK_ERROR_OUT_OF_DEVICE_MEMORY);
			STR(VK_ERROR_INITIALIZATION_FAILED);
			STR(VK_ERROR_DEVICE_LOST);
			STR(VK_ERROR_MEMORY_MAP_FAILED);
			STR(VK_ERROR_LAYER_NOT_PRESENT);
			STR(VK_ERROR_EXTENSION_NOT_PRESENT);
			STR(VK_ERROR_FEATURE_NOT_PRESENT);
			STR(VK_ERROR_INCOMPATIBLE_DRIVER);
			STR(VK_ERROR_TOO_MANY_OBJECTS);
			STR(VK_ERROR_FORMAT_NOT_SUPPORTED);
			STR(VK_ERROR_SURFACE_LOST_KHR);
			STR(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
			STR(VK_SUBOPTIMAL_KHR);
			STR(VK_ERROR_OUT_OF_DATE_KHR);
			STR(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
			STR(VK_ERROR_VALIDATION_FAILED_EXT);
			STR(VK_ERROR_INVALID_SHADER_NV);
#undef STR
			default:
				return "UNKNOWN_ERROR";
        }
    }

    std::string to_string(VkPhysicalDeviceType type)
    {
        switch (type)
		{
#define STR(r) case VK_PHYSICAL_DEVICE_TYPE_ ##r: return #r
			STR(OTHER);
			STR(INTEGRATED_GPU);
			STR(DISCRETE_GPU);
			STR(VIRTUAL_GPU);
#undef STR
			default: return "UNKNOWN_DEVICE_TYPE";
		}
    }

    VkFormat select_depth_format(VkPhysicalDevice device)
    {
        // Since all depth formats may be optional, we need to find a suitable depth format to use
		// Start with the highest precision packed format
		std::vector<VkFormat> depthFormats = {
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM
		};

		for (auto& format : depthFormats)
		{
			VkFormatProperties formatProps;
			vkGetPhysicalDeviceFormatProperties(device, format, &formatProps);
			// Format must support depth stencil attachment for optimal tiling
			if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
			{
				return format;
			}
		}

        return VK_FORMAT_UNDEFINED;
    }

    VulkanHandle<VkShaderModule> load_shader_module(VkDevice device, const std::string& path, Json::Value* reflection)
    {
        static std::vector<std::string> glslExtNames = {
            ".glsl", ".vert", ".tesc", ".tese", ".geom", ".frag", ".comp", 
            ".mesh", ".task",
            ".rgen", ".rint", ".rahit", ".rchit", ".rmiss", ".rcall",
        };
        
		std::string longExtName = path.substr(path.find("."));
        std::vector<std::string> extNames = split(longExtName, ".", true);
        std::vector<uint8_t> code;

		load_file(path, code);

        if (std::any_of(glslExtNames.begin(), glslExtNames.end(), [&] (const std::string& name) {
            return istrcmp(extNames.back(), name) == 0;
        }))
        {
            // Compile GLSL
        }
        else if (istrcmp(extNames.back(), ".spv") == 0)
        {
            return nullptr;
        }

        if (reflection && is_file(path + ".reflection"))
        {
			std::string content;

			load_file(path + ".reflection", content);

            JSONCPP_STRING error;
            Json::CharReaderBuilder builder;
            const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
            
			if (!reader->parse(content.c_str(), content.c_str() + content.size(), reflection, &error)) 
            {
                std::cerr << "!!! Failed to parse shader reflection as JSON !!!" << std::endl;
				std::cerr << error.c_str() << std::endl;
            }
        }

        VkShaderModule shaderModule = VK_NULL_HANDLE;
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = (uint32_t*)code.data();

		VK_CHECK_RESULT(vkCreateShaderModule(device, &createInfo, NULL, &shaderModule));
        return VulkanHandle<VkShaderModule>(shaderModule, [=](){ vkDestroyShaderModule(device, shaderModule, NULL); });
    }
}