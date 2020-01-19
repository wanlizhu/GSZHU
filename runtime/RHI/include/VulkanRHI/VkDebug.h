#pragma once

#include "VulkanRHI/VkUtilities.h"

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

        static void SetObjectName(VkDevice device, VkCommandPool cmdPool, const char* name);
        static void SetObjectName(VkDevice device, VkCommandBuffer cmdBuffer, const char* name);
        static void SetObjectName(VkDevice device, VkQueue queue, const char* name);
        static void SetObjectName(VkDevice device, VkImage image, const char* name);
        static void SetObjectName(VkDevice device, VkImageView imageView, const char* name);
        static void SetObjectName(VkDevice device, VkSampler sampler, const char* name);
        static void SetObjectName(VkDevice device, VkBuffer buffer, const char* name);
        static void SetObjectName(VkDevice device, VkBufferView bufferView, const char* name);
        static void SetObjectName(VkDevice device, VkDeviceMemory memory, const char* name);
        static void SetObjectName(VkDevice device, VkShaderModule shaderModule, const char* name);
        static void SetObjectName(VkDevice device, VkPipeline pipeline, const char* name);
        static void SetObjectName(VkDevice device, VkPipelineLayout pipelineLayout, const char* name);
        static void SetObjectName(VkDevice device, VkRenderPass renderPass, const char* name);
        static void SetObjectName(VkDevice device, VkFramebuffer framebuffer, const char* name);
        static void SetObjectName(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const char* name);
        static void SetObjectName(VkDevice device, VkDescriptorSet descriptorSet, const char* name);
        static void SetObjectName(VkDevice device, VkDescriptorPool descriptorPool, const char* name);
        static void SetObjectName(VkDevice device, VkSemaphore semaphore, const char* name);
        static void SetObjectName(VkDevice device, VkFence fence, const char* name);
        static void SetObjectName(VkDevice device, VkEvent _event, const char* name);

        static const char* ResultToString(VkResult errorCode);
        static const char* AccessFlagBitToString(VkAccessFlagBits bit);
        static const char* ImageLayoutToString(VkImageLayout layout);
        static std::string AccessFlagsToString(VkAccessFlags flags);
        static const char* ObjectTypeToString(VkObjectType objectType);
    };
}