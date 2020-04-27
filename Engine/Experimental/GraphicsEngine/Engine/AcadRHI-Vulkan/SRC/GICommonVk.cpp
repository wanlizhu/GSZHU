#include "GICommonVk.h"

bool operator==(const VkImageSubresourceRange& lhs, const VkImageSubresourceRange& rhs)
{
    return lhs.aspectMask == rhs.aspectMask
        && lhs.baseArrayLayer == rhs.baseArrayLayer
        && lhs.baseMipLevel == rhs.baseMipLevel
        && lhs.layerCount == rhs.layerCount
        && lhs.levelCount == rhs.levelCount;
}

bool operator==(const VkRect2D& lhs, const VkRect2D& rhs)
{
    return lhs.extent.width == rhs.extent.width &&
        lhs.extent.height == rhs.extent.height &&
        lhs.offset.x == rhs.offset.x &&
        lhs.offset.y == rhs.offset.y;
}

bool operator==(const VkViewport& lhs, const VkViewport& rhs)
{
    return lhs.x == rhs.x &&
        lhs.y == rhs.y &&
        lhs.width == rhs.width &&
        lhs.height == rhs.height &&
        lhs.minDepth == rhs.minDepth &&
        lhs.maxDepth == rhs.maxDepth;
}

namespace AutoCAD::Graphics::Engine
{
    const char* GIErrorDescVk(VkResult error)
    {
        switch (error)
        {
        case VK_SUCCESS:
            return "Success";
        case VK_NOT_READY:
            return "A fence or query has not yet completed";
        case VK_TIMEOUT:
            return "A wait operation has not completed in the specified time";
        case VK_EVENT_SET:
            return "An event is signaled";
        case VK_EVENT_RESET:
            return "An event is unsignaled";
        case VK_INCOMPLETE:
            return "A return array was too small for the result";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "A host memory allocation has failed";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "A device memory allocation has failed";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "Initialization of an object could not be completed for implementation-specific reasons";
        case VK_ERROR_DEVICE_LOST:
            return "The logical or physical device has been lost";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "Mapping of a memory object has failed";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "A requested layer is not present or could not be loaded";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "A requested extension is not supported";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "A requested feature is not supported";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "Too many objects of the type have already been created";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "A requested format is not supported on this device";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "A surface is no longer available";
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return "A allocation failed due to having no more space in the descriptor pool";
        case VK_SUBOPTIMAL_KHR:
            return "A swapchain no longer matches the surface properties exactly, but can still be used";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "A surface has changed in such a way that it is no longer compatible with the swapchain";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "The display used by a swapchain does not use the same presentable image layout";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "A validation layer found an error";
        default:
            return "Unknown Vulkan error";
        }
    }

    const char* GIFormatToStringVk(VkFormat format)
    {
#define IN_CASE_OF(name) case name: return #name
        switch (format)
        {
            IN_CASE_OF(VK_FORMAT_UNDEFINED);
            IN_CASE_OF(VK_FORMAT_R4G4_UNORM_PACK8);
            IN_CASE_OF(VK_FORMAT_R4G4B4A4_UNORM_PACK16);
            IN_CASE_OF(VK_FORMAT_B4G4R4A4_UNORM_PACK16);
            IN_CASE_OF(VK_FORMAT_R5G6B5_UNORM_PACK16);
            IN_CASE_OF(VK_FORMAT_B5G6R5_UNORM_PACK16);
            IN_CASE_OF(VK_FORMAT_R5G5B5A1_UNORM_PACK16);
            IN_CASE_OF(VK_FORMAT_B5G5R5A1_UNORM_PACK16);
            IN_CASE_OF(VK_FORMAT_A1R5G5B5_UNORM_PACK16);
            IN_CASE_OF(VK_FORMAT_R8_UNORM);
            IN_CASE_OF(VK_FORMAT_R8_SNORM);
            IN_CASE_OF(VK_FORMAT_R8_USCALED);
            IN_CASE_OF(VK_FORMAT_R8_SSCALED);
            IN_CASE_OF(VK_FORMAT_R8_UINT);
            IN_CASE_OF(VK_FORMAT_R8_SINT);
            IN_CASE_OF(VK_FORMAT_R8_SRGB);
            IN_CASE_OF(VK_FORMAT_R8G8_UNORM);
            IN_CASE_OF(VK_FORMAT_R8G8_SNORM);
            IN_CASE_OF(VK_FORMAT_R8G8_USCALED);
            IN_CASE_OF(VK_FORMAT_R8G8_SSCALED);
            IN_CASE_OF(VK_FORMAT_R8G8_UINT);
            IN_CASE_OF(VK_FORMAT_R8G8_SINT);
            IN_CASE_OF(VK_FORMAT_R8G8_SRGB);
            IN_CASE_OF(VK_FORMAT_R8G8B8_UNORM);
            IN_CASE_OF(VK_FORMAT_R8G8B8_SNORM);
            IN_CASE_OF(VK_FORMAT_R8G8B8_USCALED);
            IN_CASE_OF(VK_FORMAT_R8G8B8_SSCALED);
            IN_CASE_OF(VK_FORMAT_R8G8B8_UINT);
            IN_CASE_OF(VK_FORMAT_R8G8B8_SINT);
            IN_CASE_OF(VK_FORMAT_R8G8B8_SRGB);
            IN_CASE_OF(VK_FORMAT_B8G8R8_UNORM);
            IN_CASE_OF(VK_FORMAT_B8G8R8_SNORM);
            IN_CASE_OF(VK_FORMAT_B8G8R8_USCALED);
            IN_CASE_OF(VK_FORMAT_B8G8R8_SSCALED);
            IN_CASE_OF(VK_FORMAT_B8G8R8_UINT);
            IN_CASE_OF(VK_FORMAT_B8G8R8_SINT);
            IN_CASE_OF(VK_FORMAT_B8G8R8_SRGB);
            IN_CASE_OF(VK_FORMAT_R8G8B8A8_UNORM);
            IN_CASE_OF(VK_FORMAT_R8G8B8A8_SNORM);
            IN_CASE_OF(VK_FORMAT_R8G8B8A8_USCALED);
            IN_CASE_OF(VK_FORMAT_R8G8B8A8_SSCALED);
            IN_CASE_OF(VK_FORMAT_R8G8B8A8_UINT);
            IN_CASE_OF(VK_FORMAT_R8G8B8A8_SINT);
            IN_CASE_OF(VK_FORMAT_R8G8B8A8_SRGB);
            IN_CASE_OF(VK_FORMAT_B8G8R8A8_UNORM);
            IN_CASE_OF(VK_FORMAT_B8G8R8A8_SNORM);
            IN_CASE_OF(VK_FORMAT_B8G8R8A8_USCALED);
            IN_CASE_OF(VK_FORMAT_B8G8R8A8_SSCALED);
            IN_CASE_OF(VK_FORMAT_B8G8R8A8_UINT);
            IN_CASE_OF(VK_FORMAT_B8G8R8A8_SINT);
            IN_CASE_OF(VK_FORMAT_B8G8R8A8_SRGB);
            IN_CASE_OF(VK_FORMAT_A8B8G8R8_UNORM_PACK32);
            IN_CASE_OF(VK_FORMAT_A8B8G8R8_SNORM_PACK32);
            IN_CASE_OF(VK_FORMAT_A8B8G8R8_USCALED_PACK32);
            IN_CASE_OF(VK_FORMAT_A8B8G8R8_SSCALED_PACK32);
            IN_CASE_OF(VK_FORMAT_A8B8G8R8_UINT_PACK32);
            IN_CASE_OF(VK_FORMAT_A8B8G8R8_SINT_PACK32);
            IN_CASE_OF(VK_FORMAT_A8B8G8R8_SRGB_PACK32);
            IN_CASE_OF(VK_FORMAT_A2R10G10B10_UNORM_PACK32);
            IN_CASE_OF(VK_FORMAT_A2R10G10B10_SNORM_PACK32);
            IN_CASE_OF(VK_FORMAT_A2R10G10B10_USCALED_PACK32);
            IN_CASE_OF(VK_FORMAT_A2R10G10B10_SSCALED_PACK32);
            IN_CASE_OF(VK_FORMAT_A2R10G10B10_UINT_PACK32);
            IN_CASE_OF(VK_FORMAT_A2R10G10B10_SINT_PACK32);
            IN_CASE_OF(VK_FORMAT_A2B10G10R10_UNORM_PACK32);
            IN_CASE_OF(VK_FORMAT_A2B10G10R10_SNORM_PACK32);
            IN_CASE_OF(VK_FORMAT_A2B10G10R10_USCALED_PACK32);
            IN_CASE_OF(VK_FORMAT_A2B10G10R10_SSCALED_PACK32);
            IN_CASE_OF(VK_FORMAT_A2B10G10R10_UINT_PACK32);
            IN_CASE_OF(VK_FORMAT_A2B10G10R10_SINT_PACK32);
            IN_CASE_OF(VK_FORMAT_R16_UNORM);
            IN_CASE_OF(VK_FORMAT_R16_SNORM);
            IN_CASE_OF(VK_FORMAT_R16_USCALED);
            IN_CASE_OF(VK_FORMAT_R16_SSCALED);
            IN_CASE_OF(VK_FORMAT_R16_UINT);
            IN_CASE_OF(VK_FORMAT_R16_SINT);
            IN_CASE_OF(VK_FORMAT_R16_SFLOAT);
            IN_CASE_OF(VK_FORMAT_R16G16_UNORM);
            IN_CASE_OF(VK_FORMAT_R16G16_SNORM);
            IN_CASE_OF(VK_FORMAT_R16G16_USCALED);
            IN_CASE_OF(VK_FORMAT_R16G16_SSCALED);
            IN_CASE_OF(VK_FORMAT_R16G16_UINT);
            IN_CASE_OF(VK_FORMAT_R16G16_SINT);
            IN_CASE_OF(VK_FORMAT_R16G16_SFLOAT);
            IN_CASE_OF(VK_FORMAT_R16G16B16_UNORM);
            IN_CASE_OF(VK_FORMAT_R16G16B16_SNORM);
            IN_CASE_OF(VK_FORMAT_R16G16B16_USCALED);
            IN_CASE_OF(VK_FORMAT_R16G16B16_SSCALED);
            IN_CASE_OF(VK_FORMAT_R16G16B16_UINT);
            IN_CASE_OF(VK_FORMAT_R16G16B16_SINT);
            IN_CASE_OF(VK_FORMAT_R16G16B16_SFLOAT);
            IN_CASE_OF(VK_FORMAT_R16G16B16A16_UNORM);
            IN_CASE_OF(VK_FORMAT_R16G16B16A16_SNORM);
            IN_CASE_OF(VK_FORMAT_R16G16B16A16_USCALED);
            IN_CASE_OF(VK_FORMAT_R16G16B16A16_SSCALED);
            IN_CASE_OF(VK_FORMAT_R16G16B16A16_UINT);
            IN_CASE_OF(VK_FORMAT_R16G16B16A16_SINT);
            IN_CASE_OF(VK_FORMAT_R16G16B16A16_SFLOAT);
            IN_CASE_OF(VK_FORMAT_R32_UINT);
            IN_CASE_OF(VK_FORMAT_R32_SINT);
            IN_CASE_OF(VK_FORMAT_R32_SFLOAT);
            IN_CASE_OF(VK_FORMAT_R32G32_UINT);
            IN_CASE_OF(VK_FORMAT_R32G32_SINT);
            IN_CASE_OF(VK_FORMAT_R32G32_SFLOAT);
            IN_CASE_OF(VK_FORMAT_R32G32B32_UINT);
            IN_CASE_OF(VK_FORMAT_R32G32B32_SINT);
            IN_CASE_OF(VK_FORMAT_R32G32B32_SFLOAT);
            IN_CASE_OF(VK_FORMAT_R32G32B32A32_UINT);
            IN_CASE_OF(VK_FORMAT_R32G32B32A32_SINT);
            IN_CASE_OF(VK_FORMAT_R32G32B32A32_SFLOAT);
            IN_CASE_OF(VK_FORMAT_R64_UINT);
            IN_CASE_OF(VK_FORMAT_R64_SINT);
            IN_CASE_OF(VK_FORMAT_R64_SFLOAT);
            IN_CASE_OF(VK_FORMAT_R64G64_UINT);
            IN_CASE_OF(VK_FORMAT_R64G64_SINT);
            IN_CASE_OF(VK_FORMAT_R64G64_SFLOAT);
            IN_CASE_OF(VK_FORMAT_R64G64B64_UINT);
            IN_CASE_OF(VK_FORMAT_R64G64B64_SINT);
            IN_CASE_OF(VK_FORMAT_R64G64B64_SFLOAT);
            IN_CASE_OF(VK_FORMAT_R64G64B64A64_UINT);
            IN_CASE_OF(VK_FORMAT_R64G64B64A64_SINT);
            IN_CASE_OF(VK_FORMAT_R64G64B64A64_SFLOAT);
            IN_CASE_OF(VK_FORMAT_B10G11R11_UFLOAT_PACK32);
            IN_CASE_OF(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32);
            IN_CASE_OF(VK_FORMAT_D16_UNORM);
            IN_CASE_OF(VK_FORMAT_X8_D24_UNORM_PACK32);
            IN_CASE_OF(VK_FORMAT_D32_SFLOAT);
            IN_CASE_OF(VK_FORMAT_S8_UINT);
            IN_CASE_OF(VK_FORMAT_D16_UNORM_S8_UINT);
            IN_CASE_OF(VK_FORMAT_D24_UNORM_S8_UINT);
            IN_CASE_OF(VK_FORMAT_D32_SFLOAT_S8_UINT);
            IN_CASE_OF(VK_FORMAT_BC1_RGB_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC1_RGB_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC1_RGBA_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC1_RGBA_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC2_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC2_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC3_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC3_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC4_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC4_SNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC5_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC5_SNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC6H_UFLOAT_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC6H_SFLOAT_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC7_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_BC7_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_EAC_R11_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_EAC_R11_SNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_EAC_R11G11_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_EAC_R11G11_SNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_4x4_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_4x4_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_5x4_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_5x4_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_5x5_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_5x5_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_6x5_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_6x5_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_6x6_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_6x6_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_8x5_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_8x5_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_8x6_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_8x6_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_8x8_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_8x8_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_10x5_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_10x5_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_10x6_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_10x6_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_10x8_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_10x8_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_10x10_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_10x10_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_12x10_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_12x10_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_12x12_UNORM_BLOCK);
            IN_CASE_OF(VK_FORMAT_ASTC_12x12_SRGB_BLOCK);
            IN_CASE_OF(VK_FORMAT_G8B8G8R8_422_UNORM);
            IN_CASE_OF(VK_FORMAT_B8G8R8G8_422_UNORM);
            IN_CASE_OF(VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM);
            IN_CASE_OF(VK_FORMAT_G8_B8R8_2PLANE_420_UNORM);
            IN_CASE_OF(VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM);
            IN_CASE_OF(VK_FORMAT_G8_B8R8_2PLANE_422_UNORM);
            IN_CASE_OF(VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM);
            IN_CASE_OF(VK_FORMAT_R10X6_UNORM_PACK16);
            IN_CASE_OF(VK_FORMAT_R10X6G10X6_UNORM_2PACK16);
            IN_CASE_OF(VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16);
            IN_CASE_OF(VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16);
            IN_CASE_OF(VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16);
            IN_CASE_OF(VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16);
            IN_CASE_OF(VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16);
            IN_CASE_OF(VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16);
            IN_CASE_OF(VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16);
            IN_CASE_OF(VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16);
            IN_CASE_OF(VK_FORMAT_R12X4_UNORM_PACK16);
            IN_CASE_OF(VK_FORMAT_R12X4G12X4_UNORM_2PACK16);
            IN_CASE_OF(VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16);
            IN_CASE_OF(VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16);
            IN_CASE_OF(VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16);
            IN_CASE_OF(VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16);
            IN_CASE_OF(VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16);
            IN_CASE_OF(VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16);
            IN_CASE_OF(VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16);
            IN_CASE_OF(VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16);
            IN_CASE_OF(VK_FORMAT_G16B16G16R16_422_UNORM);
            IN_CASE_OF(VK_FORMAT_B16G16R16G16_422_UNORM);
            IN_CASE_OF(VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM);
            IN_CASE_OF(VK_FORMAT_G16_B16R16_2PLANE_420_UNORM);
            IN_CASE_OF(VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM);
            IN_CASE_OF(VK_FORMAT_G16_B16R16_2PLANE_422_UNORM);
            IN_CASE_OF(VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM);
            IN_CASE_OF(VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG);
            IN_CASE_OF(VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG);
            IN_CASE_OF(VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG);
            IN_CASE_OF(VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG);
            IN_CASE_OF(VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG);
            IN_CASE_OF(VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG);
            IN_CASE_OF(VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG);
            IN_CASE_OF(VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG);
#ifdef _WIN32
            IN_CASE_OF(VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT);
            IN_CASE_OF(VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT);
#endif
        default: return "";
        }
#undef IN_CASE_OF
    }
}