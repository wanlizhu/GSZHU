#include "vulkan_handle.h"
#include <cassert>

namespace djv
{
    void delete_vulkan_handle(
        VkDevice device,
        void* handle,
        const VkAllocationCallbacks* allocator,
        const std::type_info& type
        )
    {
        if (type.hash_code() == typeid(VkInstance).hash_code()) {
            vkDestroyInstance((VkInstance)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkDevice).hash_code()) {
            vkDestroyDevice((VkDevice)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkSemaphore).hash_code()) {
            vkDestroySemaphore(device, (VkSemaphore)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkFence).hash_code()) {
            vkDestroyFence(device, (VkFence)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkBuffer).hash_code()) {
            vkDestroyBuffer(device, (VkBuffer)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkImage).hash_code()) {
            vkDestroyImage(device, (VkImage)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkEvent).hash_code()) {
            vkDestroyEvent(device, (VkEvent)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkQueryPool).hash_code()) {
            vkDestroyQueryPool(device, (VkQueryPool)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkBufferView).hash_code()) {
            vkDestroyBufferView(device, (VkBufferView)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkImageView).hash_code()) {
            vkDestroyImageView(device, (VkImageView)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkShaderModule).hash_code()) {
            vkDestroyShaderModule(device, (VkShaderModule)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkPipelineCache).hash_code()) {
            vkDestroyPipelineCache(device, (VkPipelineCache)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkPipelineLayout).hash_code()) {
            vkDestroyPipelineLayout(device, (VkPipelineLayout)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkRenderPass).hash_code()) {
            vkDestroyRenderPass(device, (VkRenderPass)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkPipeline).hash_code()) {
            vkDestroyPipeline(device, (VkPipeline)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkDescriptorSetLayout).hash_code()) {
            vkDestroyDescriptorSetLayout(device, (VkDescriptorSetLayout)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkSampler).hash_code()) {
            vkDestroySampler(device, (VkSampler)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkDescriptorPool).hash_code()) {
            vkDestroyDescriptorPool(device, (VkDescriptorPool)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkFramebuffer).hash_code()) {
            vkDestroyFramebuffer(device, (VkFramebuffer)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkCommandPool).hash_code()) {
            vkDestroyCommandPool(device, (VkCommandPool)handle, allocator);
        }
        else if (type.hash_code() == typeid(VkSwapchainKHR).hash_code()) {
            vkDestroySwapchainKHR(device, (VkSwapchainKHR)handle, allocator);
        }
    }
}