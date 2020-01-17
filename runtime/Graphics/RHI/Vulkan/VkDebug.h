#pragma once

#include "Graphics/RHI/Vulkan/VkUtilities.h"

namespace Wanlix
{
    static const StringArray validationLayerNames;

    class VkDebug final
    {
    public:
        static void Initialize(VkInstance instance,
                               VkDebugUtilsMessageSeverityFlagsEXT messageSeverity,
                               VkDebugUtilsMessageTypeFlagsEXT messageType,
                               void* userData = nullptr);
        static void Destroy(VkInstance instance);

        static void BeginCommandQueueLabelRegion(VkQueue queue, const String& label, const Float4& color);
        static void EndCommandQueueLabelRegion(VkQueue queue);
        static void InsertCommandQueueLabel(VkQueue queue, const String& label, const Float4& color);
        static void BeginCommandBufferLabelRegion(VkCommandBuffer cmdBuffer, const String& label, const Float4& color);
        static void EndCommandBufferLabelRegion(VkCommandBuffer cmdBuffer);
        static void InsertCommandBufferLabelRegion(VkCommandBuffer cmdBuffer, const String& label, const Float4& color);

        static const char* ResultToString(VkResult errorCode);
        static const char* AccessFlagBitToString(VkAccessFlagBits bit);
        static const char* ImageLayoutToString(VkImageLayout layout);
        static std::string AccessFlagsToString(VkAccessFlags flags);
        static const char* ObjectTypeToString(VkObjectType objectType);
    };
}