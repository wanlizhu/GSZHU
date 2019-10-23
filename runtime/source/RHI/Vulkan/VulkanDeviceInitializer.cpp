#include "VulkanDevice.h"
#include <iostream>

namespace GE2::RHI
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL __debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                          VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                          const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                          void* pUserData) 
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    void SetupDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* info)
    {
        info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        info->pfnUserCallback = __debugCallback;
        info->pUserData = nullptr;
    }

    LIST<bool> CheckValidationLayerSupport(const STRLIST& layerNames)
    {
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        LIST<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        LIST<bool> result(layerNames.size(), false);
        int i = 0;
        for (const STR& layerName : layerNames)
        {
            auto it = std::find_if(availableLayers.begin(), availableLayers.end(), 
                                   [&](const VkLayerProperties& props) {
                                       return std::strcmp(props.layerName, layerName.c_str()) == 0;
                                   });
            result[i++] = it != availableLayers.end();
        }

        return result;
    }

    std::pair<VkInstance, VkDebugUtilsMessengerEXT> 
    CreateInstance(const STRLIST& extensions, 
                   const STRLIST& layers,
                   bool enableValidationLayers)
    {
        using ResultType = std::pair<VkInstance, VkDebugUtilsMessengerEXT>;
        
        UPTR<CSTR> extensions_cstrs = Unpack(extensions);
        UPTR<CSTR> layers_cstrs = Unpack(layers);
        VkDebugUtilsMessengerCreateInfoEXT messengerInfo = {};

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Unnamed";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "GE2";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions_cstrs.get();
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
        createInfo.pNext = nullptr;

        if (enableValidationLayers)
        {
            if (!ALL_TRUE(CheckValidationLayerSupport(layers)))
            {
                printf("error @VkInstance :validation layers requested, but not available");
                return ResultType();
            }

            createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
            createInfo.ppEnabledLayerNames = layers_cstrs.get();

            SetupDebugMessengerCreateInfo(&messengerInfo);
            createInfo.pNext = &messengerInfo;
        }

        VkInstance instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            printf("error @Vulkan :failed to create instance");
            return ResultType();
        }
        
        if (enableValidationLayers)
        {
            if (!INSTANCE_PROC(instance, vkCreateDebugUtilsMessengerEXT)(instance, &messengerInfo, nullptr, &debugMessenger))
            {
                printf("error @Vulkan :failed to set up debug messenger");
                return std::pair<VkInstance, VkDebugUtilsMessengerEXT>();
            }
        }

        return std::make_pair(instance, debugMessenger);
    }

    VkPhysicalDevice SelectPhysicalDevice(VkInstance instance,
                                          VkPhysicalDeviceFeatures features)
    {

    }

    VkPhysicalDeviceFeatures Unpack(const LIST<EDeviceFeature>& features)
    {
        VkPhysicalDeviceFeatures result = {};
        for (EDeviceFeature feature : features)
        {
            switch (feature)
            {
            case EDeviceFeature::RobustBufferAccess: result.robustBufferAccess = true; break;
            case EDeviceFeature::FullDrawIndexUint32: result.fullDrawIndexUint32 = true; break;
            case EDeviceFeature::ImageCubeArray: result.imageCubeArray = true; break;
            case EDeviceFeature::IndependentBlend: result.independentBlend = true; break;
            case EDeviceFeature::GeometryShader: result.geometryShader = true; break;
            case EDeviceFeature::TessellationShader: result.tessellationShader = true; break;
            case EDeviceFeature::SampleRateShading: result.sampleRateShading = true; break;
            case EDeviceFeature::DualSrcBlend: result.dualSrcBlend = true; break;
            case EDeviceFeature::LogicOp: result.logicOp = true; break;
            case EDeviceFeature::MultiDrawIndirect: result.multiDrawIndirect = true; break;
            case EDeviceFeature::DrawIndirectFirstInstance: result.drawIndirectFirstInstance = true; break;
            case EDeviceFeature::DepthClamp: result.depthClamp = true; break;
            case EDeviceFeature::DepthBiasClamp: result.depthBiasClamp = true; break;
            case EDeviceFeature::FillModeNonSolid: result.fillModeNonSolid = true; break;
            case EDeviceFeature::DepthBounds: result.depthBounds = true; break;
            case EDeviceFeature::WideLines: result.wideLines = true; break;
            case EDeviceFeature::LargePoints: result.largePoints = true; break;
            case EDeviceFeature::AlphaToOne: result.alphaToOne = true; break;
            case EDeviceFeature::MultiViewport: result.multiViewport = true; break;
            case EDeviceFeature::SamplerAnisotropy: result.samplerAnisotropy = true; break;
            case EDeviceFeature::TextureCompressionETC2: result.textureCompressionETC2 = true; break;
            case EDeviceFeature::TextureCompressionASTC_LDR: result.textureCompressionASTC_LDR = true; break;
            case EDeviceFeature::TextureCompressionBC: result.textureCompressionBC = true; break;
            case EDeviceFeature::OcclusionQueryPrecise: result.occlusionQueryPrecise = true; break;
            case EDeviceFeature::PipelineStatisticsQuery: result.pipelineStatisticsQuery = true; break;
            case EDeviceFeature::VertexPipelineStoresAndAtomics: result.vertexPipelineStoresAndAtomics = true; break;
            case EDeviceFeature::FragmentStoresAndAtomics: result.fragmentStoresAndAtomics = true; break;
            case EDeviceFeature::ShaderTessellationAndGeometryPointSize: result.shaderTessellationAndGeometryPointSize = true; break;
            case EDeviceFeature::ShaderImageGatherExtended: result.shaderImageGatherExtended = true; break;
            case EDeviceFeature::ShaderStorageImageExtendedFormats: result.shaderStorageImageExtendedFormats = true; break;
            case EDeviceFeature::ShaderStorageImageMultisample: result.shaderStorageImageMultisample = true; break;
            case EDeviceFeature::ShaderStorageImageReadWithoutFormat: result.shaderStorageImageReadWithoutFormat = true; break;
            case EDeviceFeature::ShaderStorageImageWriteWithoutFormat: result.shaderStorageImageWriteWithoutFormat = true; break;
            case EDeviceFeature::ShaderUniformBufferArrayDynamicIndexing: result.shaderUniformBufferArrayDynamicIndexing = true; break;
            case EDeviceFeature::ShaderSampledImageArrayDynamicIndexing: result.shaderSampledImageArrayDynamicIndexing = true; break;
            case EDeviceFeature::ShaderStorageBufferArrayDynamicIndexing: result.shaderStorageBufferArrayDynamicIndexing = true; break;
            case EDeviceFeature::ShaderStorageImageArrayDynamicIndexing: result.shaderStorageImageArrayDynamicIndexing = true; break;
            case EDeviceFeature::ShaderClipDistance: result.shaderClipDistance = true; break;
            case EDeviceFeature::ShaderCullDistance: result.shaderCullDistance = true; break;
            case EDeviceFeature::ShaderFloat64: result.shaderFloat64 = true; break;
            case EDeviceFeature::ShaderInt64: result.shaderInt64 = true; break;
            case EDeviceFeature::ShaderInt16: result.shaderInt16 = true; break;
            case EDeviceFeature::ShaderResourceResidency: result.shaderResourceResidency = true; break;
            case EDeviceFeature::ShaderResourceMinLod: result.shaderResourceMinLod = true; break;
            case EDeviceFeature::SparseBinding: result.sparseBinding = true; break;
            case EDeviceFeature::SparseResidencyBuffer: result.sparseResidencyBuffer = true; break;
            case EDeviceFeature::SparseResidencyImage2D: result.sparseResidencyImage2D = true; break;
            case EDeviceFeature::SparseResidencyImage3D: result.sparseResidencyImage3D = true; break;
            case EDeviceFeature::SparseResidency2Samples: result.sparseResidency2Samples = true; break;
            case EDeviceFeature::SparseResidency4Samples: result.sparseResidency4Samples = true; break;
            case EDeviceFeature::SparseResidency8Samples: result.sparseResidency8Samples = true; break;
            case EDeviceFeature::SparseResidency16Samples: result.sparseResidency16Samples = true; break;
            case EDeviceFeature::SparseResidencyAliased: result.sparseResidencyAliased = true; break;
            case EDeviceFeature::VariableMultisampleRate: result.variableMultisampleRate = true; break;
            case EDeviceFeature::InheritedQueries: result.inheritedQueries = true; break;
            default:
            }
        }
        return result;
    }
}