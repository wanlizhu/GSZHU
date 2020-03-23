#if defined(__ANDROID__)

#include "GIAndroidPlatformVk.h"
#include "GIPlatformVk.h"
#include <dlfcn.h>

namespace AutoCAD::Graphics::Engine
{
    static void* libvulkan = nullptr;

    void GIAndroidPlatformVk::LoadVulkanLibrary()
    {
        // Load vulkan library
        libvulkan = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
        if (!libvulkan)
        {
            LOG_ERROR("Could not load vulkan library : %s!\n", dlerror());
            return false;
        }

        LoadVulkanFunctions();
        return true;
    }

    void GIAndroidPlatformVk::FreeVulkanLibrary()
    {
        if (libvulkan)
        {
            flclose(libvulkan);
            libvulkan = nullptr;
        }
    }

    std::vector<const char*> const& GIAndroidPlatformVk::RequiredInstanceLayers()
    {
        static std::vector<const char*> layers;
        if (layers.empty())
        {   /*
             * Use of the meta-layer is no longer recommended,
             * please replace references to VK_LAYER_LUNARG_standard_validation with VK_LAYER_KHRONOS_validation
            */
            layers.push_back("VK_LAYER_KHRONOS_validation");
        }
        return layers;
    }

    std::vector<const char*> const& GIAndroidPlatformVk::RequiredInstanceExtensions()
    {
        static std::vector<const char*> extensions;
        if (extensions.empty())
        {
            extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
            extensions.push_back(VK_GOOGLE_DISPLAY_TIMING_EXTENSION_NAME);
        }
        return extensions;
    }

    std::vector<const char*> const& GIAndroidPlatformVk::RequiredDeviceExtensions(EGPUVendorID vendorId)
    {
        static std::vector<const char*> extensions;
        if (extensions.empty())
        {
            extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
            extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            extensions.push_back(VK_GOOGLE_DISPLAY_TIMING_EXTENSION_NAME);
            extensions.push_back(VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME);
        }
        return extensions;
    }

    VkPhysicalDeviceFeatures const& GIAndroidPlatformVk::RequiredDeviceFeatures()
    {
        static VkPhysicalDeviceFeatures features = {};
        return features;
    }

    void GIAndroidPlatformVk::CreateSurface(VkInstance instance, void* window, VkSurfaceKHR* surface)
    {
        VkAndroidSurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.window = (ANativeWindow*)window;
        VK_CHECK(vkCreateAndroidSurfaceKHR(instance, &createInfo, nullptr, surface));
    }

    void GIAndroidPlatformVk::LoadVulkanFunctions()
    {
        // Vulkan supported, set function addresses
        vkCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(dlsym(libvulkan, "vkCreateInstance"));
        vkDestroyInstance = reinterpret_cast<PFN_vkDestroyInstance>(dlsym(libvulkan, "vkDestroyInstance"));
        vkEnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(dlsym(libvulkan, "vkEnumeratePhysicalDevices"));
        vkGetPhysicalDeviceFeatures = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures>(dlsym(libvulkan, "vkGetPhysicalDeviceFeatures"));
        vkGetPhysicalDeviceFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceFormatProperties>(dlsym(libvulkan, "vkGetPhysicalDeviceFormatProperties"));
        vkGetPhysicalDeviceImageFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceImageFormatProperties>(dlsym(libvulkan, "vkGetPhysicalDeviceImageFormatProperties"));
        vkGetPhysicalDeviceProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties>(dlsym(libvulkan, "vkGetPhysicalDeviceProperties"));
        vkGetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(dlsym(libvulkan, "vkGetPhysicalDeviceQueueFamilyProperties"));
        vkGetPhysicalDeviceMemoryProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceMemoryProperties>(dlsym(libvulkan, "vkGetPhysicalDeviceMemoryProperties"));
        vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(dlsym(libvulkan, "vkGetInstanceProcAddr"));
        vkGetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(dlsym(libvulkan, "vkGetDeviceProcAddr"));
        vkCreateDevice = reinterpret_cast<PFN_vkCreateDevice>(dlsym(libvulkan, "vkCreateDevice"));
        vkDestroyDevice = reinterpret_cast<PFN_vkDestroyDevice>(dlsym(libvulkan, "vkDestroyDevice"));
        vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(dlsym(libvulkan, "vkEnumerateInstanceExtensionProperties"));
        vkEnumerateDeviceExtensionProperties = reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(dlsym(libvulkan, "vkEnumerateDeviceExtensionProperties"));
        vkEnumerateInstanceLayerProperties = reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(dlsym(libvulkan, "vkEnumerateInstanceLayerProperties"));
        vkEnumerateDeviceLayerProperties = reinterpret_cast<PFN_vkEnumerateDeviceLayerProperties>(dlsym(libvulkan, "vkEnumerateDeviceLayerProperties"));
        vkGetDeviceQueue = reinterpret_cast<PFN_vkGetDeviceQueue>(dlsym(libvulkan, "vkGetDeviceQueue"));
        vkQueueSubmit = reinterpret_cast<PFN_vkQueueSubmit>(dlsym(libvulkan, "vkQueueSubmit"));
        vkQueueWaitIdle = reinterpret_cast<PFN_vkQueueWaitIdle>(dlsym(libvulkan, "vkQueueWaitIdle"));
        vkDeviceWaitIdle = reinterpret_cast<PFN_vkDeviceWaitIdle>(dlsym(libvulkan, "vkDeviceWaitIdle"));
        vkAllocateMemory = reinterpret_cast<PFN_vkAllocateMemory>(dlsym(libvulkan, "vkAllocateMemory"));
        vkFreeMemory = reinterpret_cast<PFN_vkFreeMemory>(dlsym(libvulkan, "vkFreeMemory"));
        vkMapMemory = reinterpret_cast<PFN_vkMapMemory>(dlsym(libvulkan, "vkMapMemory"));
        vkUnmapMemory = reinterpret_cast<PFN_vkUnmapMemory>(dlsym(libvulkan, "vkUnmapMemory"));
        vkFlushMappedMemoryRanges = reinterpret_cast<PFN_vkFlushMappedMemoryRanges>(dlsym(libvulkan, "vkFlushMappedMemoryRanges"));
        vkInvalidateMappedMemoryRanges = reinterpret_cast<PFN_vkInvalidateMappedMemoryRanges>(dlsym(libvulkan, "vkInvalidateMappedMemoryRanges"));
        vkGetDeviceMemoryCommitment = reinterpret_cast<PFN_vkGetDeviceMemoryCommitment>(dlsym(libvulkan, "vkGetDeviceMemoryCommitment"));
        vkBindBufferMemory = reinterpret_cast<PFN_vkBindBufferMemory>(dlsym(libvulkan, "vkBindBufferMemory"));
        vkBindImageMemory = reinterpret_cast<PFN_vkBindImageMemory>(dlsym(libvulkan, "vkBindImageMemory"));
        vkGetBufferMemoryRequirements = reinterpret_cast<PFN_vkGetBufferMemoryRequirements>(dlsym(libvulkan, "vkGetBufferMemoryRequirements"));
        vkGetImageMemoryRequirements = reinterpret_cast<PFN_vkGetImageMemoryRequirements>(dlsym(libvulkan, "vkGetImageMemoryRequirements"));
        vkGetImageSparseMemoryRequirements = reinterpret_cast<PFN_vkGetImageSparseMemoryRequirements>(dlsym(libvulkan, "vkGetImageSparseMemoryRequirements"));
        vkGetPhysicalDeviceSparseImageFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceSparseImageFormatProperties>(dlsym(libvulkan, "vkGetPhysicalDeviceSparseImageFormatProperties"));
        vkQueueBindSparse = reinterpret_cast<PFN_vkQueueBindSparse>(dlsym(libvulkan, "vkQueueBindSparse"));
        vkCreateFence = reinterpret_cast<PFN_vkCreateFence>(dlsym(libvulkan, "vkCreateFence"));
        vkDestroyFence = reinterpret_cast<PFN_vkDestroyFence>(dlsym(libvulkan, "vkDestroyFence"));
        vkResetFences = reinterpret_cast<PFN_vkResetFences>(dlsym(libvulkan, "vkResetFences"));
        vkGetFenceStatus = reinterpret_cast<PFN_vkGetFenceStatus>(dlsym(libvulkan, "vkGetFenceStatus"));
        vkWaitForFences = reinterpret_cast<PFN_vkWaitForFences>(dlsym(libvulkan, "vkWaitForFences"));
        vkCreateSemaphore = reinterpret_cast<PFN_vkCreateSemaphore>(dlsym(libvulkan, "vkCreateSemaphore"));
        vkDestroySemaphore = reinterpret_cast<PFN_vkDestroySemaphore>(dlsym(libvulkan, "vkDestroySemaphore"));
        vkCreateEvent = reinterpret_cast<PFN_vkCreateEvent>(dlsym(libvulkan, "vkCreateEvent"));
        vkDestroyEvent = reinterpret_cast<PFN_vkDestroyEvent>(dlsym(libvulkan, "vkDestroyEvent"));
        vkGetEventStatus = reinterpret_cast<PFN_vkGetEventStatus>(dlsym(libvulkan, "vkGetEventStatus"));
        vkSetEvent = reinterpret_cast<PFN_vkSetEvent>(dlsym(libvulkan, "vkSetEvent"));
        vkResetEvent = reinterpret_cast<PFN_vkResetEvent>(dlsym(libvulkan, "vkResetEvent"));
        vkCreateQueryPool = reinterpret_cast<PFN_vkCreateQueryPool>(dlsym(libvulkan, "vkCreateQueryPool"));
        vkDestroyQueryPool = reinterpret_cast<PFN_vkDestroyQueryPool>(dlsym(libvulkan, "vkDestroyQueryPool"));
        vkGetQueryPoolResults = reinterpret_cast<PFN_vkGetQueryPoolResults>(dlsym(libvulkan, "vkGetQueryPoolResults"));
        vkCreateBuffer = reinterpret_cast<PFN_vkCreateBuffer>(dlsym(libvulkan, "vkCreateBuffer"));
        vkDestroyBuffer = reinterpret_cast<PFN_vkDestroyBuffer>(dlsym(libvulkan, "vkDestroyBuffer"));
        vkCreateBufferView = reinterpret_cast<PFN_vkCreateBufferView>(dlsym(libvulkan, "vkCreateBufferView"));
        vkDestroyBufferView = reinterpret_cast<PFN_vkDestroyBufferView>(dlsym(libvulkan, "vkDestroyBufferView"));
        vkCreateImage = reinterpret_cast<PFN_vkCreateImage>(dlsym(libvulkan, "vkCreateImage"));
        vkDestroyImage = reinterpret_cast<PFN_vkDestroyImage>(dlsym(libvulkan, "vkDestroyImage"));
        vkGetImageSubresourceLayout = reinterpret_cast<PFN_vkGetImageSubresourceLayout>(dlsym(libvulkan, "vkGetImageSubresourceLayout"));
        vkCreateImageView = reinterpret_cast<PFN_vkCreateImageView>(dlsym(libvulkan, "vkCreateImageView"));
        vkDestroyImageView = reinterpret_cast<PFN_vkDestroyImageView>(dlsym(libvulkan, "vkDestroyImageView"));
        vkCreateShaderModule = reinterpret_cast<PFN_vkCreateShaderModule>(dlsym(libvulkan, "vkCreateShaderModule"));
        vkDestroyShaderModule = reinterpret_cast<PFN_vkDestroyShaderModule>(dlsym(libvulkan, "vkDestroyShaderModule"));
        vkCreatePipelineCache = reinterpret_cast<PFN_vkCreatePipelineCache>(dlsym(libvulkan, "vkCreatePipelineCache"));
        vkDestroyPipelineCache = reinterpret_cast<PFN_vkDestroyPipelineCache>(dlsym(libvulkan, "vkDestroyPipelineCache"));
        vkGetPipelineCacheData = reinterpret_cast<PFN_vkGetPipelineCacheData>(dlsym(libvulkan, "vkGetPipelineCacheData"));
        vkMergePipelineCaches = reinterpret_cast<PFN_vkMergePipelineCaches>(dlsym(libvulkan, "vkMergePipelineCaches"));
        vkCreateGraphicsPipelines = reinterpret_cast<PFN_vkCreateGraphicsPipelines>(dlsym(libvulkan, "vkCreateGraphicsPipelines"));
        vkCreateComputePipelines = reinterpret_cast<PFN_vkCreateComputePipelines>(dlsym(libvulkan, "vkCreateComputePipelines"));
        vkDestroyPipeline = reinterpret_cast<PFN_vkDestroyPipeline>(dlsym(libvulkan, "vkDestroyPipeline"));
        vkCreatePipelineLayout = reinterpret_cast<PFN_vkCreatePipelineLayout>(dlsym(libvulkan, "vkCreatePipelineLayout"));
        vkDestroyPipelineLayout = reinterpret_cast<PFN_vkDestroyPipelineLayout>(dlsym(libvulkan, "vkDestroyPipelineLayout"));
        vkCreateSampler = reinterpret_cast<PFN_vkCreateSampler>(dlsym(libvulkan, "vkCreateSampler"));
        vkDestroySampler = reinterpret_cast<PFN_vkDestroySampler>(dlsym(libvulkan, "vkDestroySampler"));
        vkCreateDescriptorSetLayout = reinterpret_cast<PFN_vkCreateDescriptorSetLayout>(dlsym(libvulkan, "vkCreateDescriptorSetLayout"));
        vkDestroyDescriptorSetLayout = reinterpret_cast<PFN_vkDestroyDescriptorSetLayout>(dlsym(libvulkan, "vkDestroyDescriptorSetLayout"));
        vkCreateDescriptorPool = reinterpret_cast<PFN_vkCreateDescriptorPool>(dlsym(libvulkan, "vkCreateDescriptorPool"));
        vkDestroyDescriptorPool = reinterpret_cast<PFN_vkDestroyDescriptorPool>(dlsym(libvulkan, "vkDestroyDescriptorPool"));
        vkResetDescriptorPool = reinterpret_cast<PFN_vkResetDescriptorPool>(dlsym(libvulkan, "vkResetDescriptorPool"));
        vkAllocateDescriptorSets = reinterpret_cast<PFN_vkAllocateDescriptorSets>(dlsym(libvulkan, "vkAllocateDescriptorSets"));
        vkFreeDescriptorSets = reinterpret_cast<PFN_vkFreeDescriptorSets>(dlsym(libvulkan, "vkFreeDescriptorSets"));
        vkUpdateDescriptorSets = reinterpret_cast<PFN_vkUpdateDescriptorSets>(dlsym(libvulkan, "vkUpdateDescriptorSets"));
        vkCreateFramebuffer = reinterpret_cast<PFN_vkCreateFramebuffer>(dlsym(libvulkan, "vkCreateFramebuffer"));
        vkDestroyFramebuffer = reinterpret_cast<PFN_vkDestroyFramebuffer>(dlsym(libvulkan, "vkDestroyFramebuffer"));
        vkCreateRenderPass = reinterpret_cast<PFN_vkCreateRenderPass>(dlsym(libvulkan, "vkCreateRenderPass"));
        vkDestroyRenderPass = reinterpret_cast<PFN_vkDestroyRenderPass>(dlsym(libvulkan, "vkDestroyRenderPass"));
        vkGetRenderAreaGranularity = reinterpret_cast<PFN_vkGetRenderAreaGranularity>(dlsym(libvulkan, "vkGetRenderAreaGranularity"));
        vkCreateCommandPool = reinterpret_cast<PFN_vkCreateCommandPool>(dlsym(libvulkan, "vkCreateCommandPool"));
        vkDestroyCommandPool = reinterpret_cast<PFN_vkDestroyCommandPool>(dlsym(libvulkan, "vkDestroyCommandPool"));
        vkResetCommandPool = reinterpret_cast<PFN_vkResetCommandPool>(dlsym(libvulkan, "vkResetCommandPool"));
        vkAllocateCommandBuffers = reinterpret_cast<PFN_vkAllocateCommandBuffers>(dlsym(libvulkan, "vkAllocateCommandBuffers"));
        vkFreeCommandBuffers = reinterpret_cast<PFN_vkFreeCommandBuffers>(dlsym(libvulkan, "vkFreeCommandBuffers"));
        vkBeginCommandBuffer = reinterpret_cast<PFN_vkBeginCommandBuffer>(dlsym(libvulkan, "vkBeginCommandBuffer"));
        vkEndCommandBuffer = reinterpret_cast<PFN_vkEndCommandBuffer>(dlsym(libvulkan, "vkEndCommandBuffer"));
        vkResetCommandBuffer = reinterpret_cast<PFN_vkResetCommandBuffer>(dlsym(libvulkan, "vkResetCommandBuffer"));
        vkCmdBindPipeline = reinterpret_cast<PFN_vkCmdBindPipeline>(dlsym(libvulkan, "vkCmdBindPipeline"));
        vkCmdSetViewport = reinterpret_cast<PFN_vkCmdSetViewport>(dlsym(libvulkan, "vkCmdSetViewport"));
        vkCmdSetScissor = reinterpret_cast<PFN_vkCmdSetScissor>(dlsym(libvulkan, "vkCmdSetScissor"));
        vkCmdSetLineWidth = reinterpret_cast<PFN_vkCmdSetLineWidth>(dlsym(libvulkan, "vkCmdSetLineWidth"));
        vkCmdSetDepthBias = reinterpret_cast<PFN_vkCmdSetDepthBias>(dlsym(libvulkan, "vkCmdSetDepthBias"));
        vkCmdSetBlendConstants = reinterpret_cast<PFN_vkCmdSetBlendConstants>(dlsym(libvulkan, "vkCmdSetBlendConstants"));
        vkCmdSetDepthBounds = reinterpret_cast<PFN_vkCmdSetDepthBounds>(dlsym(libvulkan, "vkCmdSetDepthBounds"));
        vkCmdSetStencilCompareMask = reinterpret_cast<PFN_vkCmdSetStencilCompareMask>(dlsym(libvulkan, "vkCmdSetStencilCompareMask"));
        vkCmdSetStencilWriteMask = reinterpret_cast<PFN_vkCmdSetStencilWriteMask>(dlsym(libvulkan, "vkCmdSetStencilWriteMask"));
        vkCmdSetStencilReference = reinterpret_cast<PFN_vkCmdSetStencilReference>(dlsym(libvulkan, "vkCmdSetStencilReference"));
        vkCmdBindDescriptorSets = reinterpret_cast<PFN_vkCmdBindDescriptorSets>(dlsym(libvulkan, "vkCmdBindDescriptorSets"));
        vkCmdBindIndexBuffer = reinterpret_cast<PFN_vkCmdBindIndexBuffer>(dlsym(libvulkan, "vkCmdBindIndexBuffer"));
        vkCmdBindVertexBuffers = reinterpret_cast<PFN_vkCmdBindVertexBuffers>(dlsym(libvulkan, "vkCmdBindVertexBuffers"));
        vkCmdDraw = reinterpret_cast<PFN_vkCmdDraw>(dlsym(libvulkan, "vkCmdDraw"));
        vkCmdDrawIndexed = reinterpret_cast<PFN_vkCmdDrawIndexed>(dlsym(libvulkan, "vkCmdDrawIndexed"));
        vkCmdDrawIndirect = reinterpret_cast<PFN_vkCmdDrawIndirect>(dlsym(libvulkan, "vkCmdDrawIndirect"));
        vkCmdDrawIndexedIndirect = reinterpret_cast<PFN_vkCmdDrawIndexedIndirect>(dlsym(libvulkan, "vkCmdDrawIndexedIndirect"));
        vkCmdDispatch = reinterpret_cast<PFN_vkCmdDispatch>(dlsym(libvulkan, "vkCmdDispatch"));
        vkCmdDispatchIndirect = reinterpret_cast<PFN_vkCmdDispatchIndirect>(dlsym(libvulkan, "vkCmdDispatchIndirect"));
        vkCmdCopyBuffer = reinterpret_cast<PFN_vkCmdCopyBuffer>(dlsym(libvulkan, "vkCmdCopyBuffer"));
        vkCmdCopyImage = reinterpret_cast<PFN_vkCmdCopyImage>(dlsym(libvulkan, "vkCmdCopyImage"));
        vkCmdBlitImage = reinterpret_cast<PFN_vkCmdBlitImage>(dlsym(libvulkan, "vkCmdBlitImage"));
        vkCmdCopyBufferToImage = reinterpret_cast<PFN_vkCmdCopyBufferToImage>(dlsym(libvulkan, "vkCmdCopyBufferToImage"));
        vkCmdCopyImageToBuffer = reinterpret_cast<PFN_vkCmdCopyImageToBuffer>(dlsym(libvulkan, "vkCmdCopyImageToBuffer"));
        vkCmdUpdateBuffer = reinterpret_cast<PFN_vkCmdUpdateBuffer>(dlsym(libvulkan, "vkCmdUpdateBuffer"));
        vkCmdFillBuffer = reinterpret_cast<PFN_vkCmdFillBuffer>(dlsym(libvulkan, "vkCmdFillBuffer"));
        vkCmdClearColorImage = reinterpret_cast<PFN_vkCmdClearColorImage>(dlsym(libvulkan, "vkCmdClearColorImage"));
        vkCmdClearDepthStencilImage = reinterpret_cast<PFN_vkCmdClearDepthStencilImage>(dlsym(libvulkan, "vkCmdClearDepthStencilImage"));
        vkCmdClearAttachments = reinterpret_cast<PFN_vkCmdClearAttachments>(dlsym(libvulkan, "vkCmdClearAttachments"));
        vkCmdResolveImage = reinterpret_cast<PFN_vkCmdResolveImage>(dlsym(libvulkan, "vkCmdResolveImage"));
        vkCmdSetEvent = reinterpret_cast<PFN_vkCmdSetEvent>(dlsym(libvulkan, "vkCmdSetEvent"));
        vkCmdResetEvent = reinterpret_cast<PFN_vkCmdResetEvent>(dlsym(libvulkan, "vkCmdResetEvent"));
        vkCmdWaitEvents = reinterpret_cast<PFN_vkCmdWaitEvents>(dlsym(libvulkan, "vkCmdWaitEvents"));
        vkCmdPipelineBarrier = reinterpret_cast<PFN_vkCmdPipelineBarrier>(dlsym(libvulkan, "vkCmdPipelineBarrier"));
        vkCmdBeginQuery = reinterpret_cast<PFN_vkCmdBeginQuery>(dlsym(libvulkan, "vkCmdBeginQuery"));
        vkCmdEndQuery = reinterpret_cast<PFN_vkCmdEndQuery>(dlsym(libvulkan, "vkCmdEndQuery"));
        vkCmdResetQueryPool = reinterpret_cast<PFN_vkCmdResetQueryPool>(dlsym(libvulkan, "vkCmdResetQueryPool"));
        vkCmdWriteTimestamp = reinterpret_cast<PFN_vkCmdWriteTimestamp>(dlsym(libvulkan, "vkCmdWriteTimestamp"));
        vkCmdCopyQueryPoolResults = reinterpret_cast<PFN_vkCmdCopyQueryPoolResults>(dlsym(libvulkan, "vkCmdCopyQueryPoolResults"));
        vkCmdPushConstants = reinterpret_cast<PFN_vkCmdPushConstants>(dlsym(libvulkan, "vkCmdPushConstants"));
        vkCmdBeginRenderPass = reinterpret_cast<PFN_vkCmdBeginRenderPass>(dlsym(libvulkan, "vkCmdBeginRenderPass"));
        vkCmdNextSubpass = reinterpret_cast<PFN_vkCmdNextSubpass>(dlsym(libvulkan, "vkCmdNextSubpass"));
        vkCmdEndRenderPass = reinterpret_cast<PFN_vkCmdEndRenderPass>(dlsym(libvulkan, "vkCmdEndRenderPass"));
        vkCmdExecuteCommands = reinterpret_cast<PFN_vkCmdExecuteCommands>(dlsym(libvulkan, "vkCmdExecuteCommands"));
        vkDestroySurfaceKHR = reinterpret_cast<PFN_vkDestroySurfaceKHR>(dlsym(libvulkan, "vkDestroySurfaceKHR"));
        vkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(dlsym(libvulkan, "vkGetPhysicalDeviceSurfaceSupportKHR"));
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(dlsym(libvulkan, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
        vkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(dlsym(libvulkan, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
        vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(dlsym(libvulkan, "vkGetPhysicalDeviceSurfacePresentModesKHR"));
        vkCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(dlsym(libvulkan, "vkCreateSwapchainKHR"));
        vkDestroySwapchainKHR = reinterpret_cast<PFN_vkDestroySwapchainKHR>(dlsym(libvulkan, "vkDestroySwapchainKHR"));
        vkGetSwapchainImagesKHR = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(dlsym(libvulkan, "vkGetSwapchainImagesKHR"));
        vkAcquireNextImageKHR = reinterpret_cast<PFN_vkAcquireNextImageKHR>(dlsym(libvulkan, "vkAcquireNextImageKHR"));
        vkQueuePresentKHR = reinterpret_cast<PFN_vkQueuePresentKHR>(dlsym(libvulkan, "vkQueuePresentKHR"));
        vkGetPhysicalDeviceDisplayPropertiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceDisplayPropertiesKHR>(dlsym(libvulkan, "vkGetPhysicalDeviceDisplayPropertiesKHR"));
        vkGetPhysicalDeviceDisplayPlanePropertiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR>(dlsym(libvulkan, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR"));
        vkGetDisplayPlaneSupportedDisplaysKHR = reinterpret_cast<PFN_vkGetDisplayPlaneSupportedDisplaysKHR>(dlsym(libvulkan, "vkGetDisplayPlaneSupportedDisplaysKHR"));
        vkGetDisplayModePropertiesKHR = reinterpret_cast<PFN_vkGetDisplayModePropertiesKHR>(dlsym(libvulkan, "vkGetDisplayModePropertiesKHR"));
        vkCreateDisplayModeKHR = reinterpret_cast<PFN_vkCreateDisplayModeKHR>(dlsym(libvulkan, "vkCreateDisplayModeKHR"));
        vkGetDisplayPlaneCapabilitiesKHR = reinterpret_cast<PFN_vkGetDisplayPlaneCapabilitiesKHR>(dlsym(libvulkan, "vkGetDisplayPlaneCapabilitiesKHR"));
        vkCreateDisplayPlaneSurfaceKHR = reinterpret_cast<PFN_vkCreateDisplayPlaneSurfaceKHR>(dlsym(libvulkan, "vkCreateDisplayPlaneSurfaceKHR"));
        vkCreateSharedSwapchainsKHR = reinterpret_cast<PFN_vkCreateSharedSwapchainsKHR>(dlsym(libvulkan, "vkCreateSharedSwapchainsKHR"));

#ifdef VK_USE_PLATFORM_XLIB_KHR
        vkCreateXlibSurfaceKHR = reinterpret_cast<PFN_vkCreateXlibSurfaceKHR>(dlsym(libvulkan, "vkCreateXlibSurfaceKHR"));
        vkGetPhysicalDeviceXlibPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR>(dlsym(libvulkan, "vkGetPhysicalDeviceXlibPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
        vkCreateXcbSurfaceKHR = reinterpret_cast<PFN_vkCreateXcbSurfaceKHR>(dlsym(libvulkan, "vkCreateXcbSurfaceKHR"));
        vkGetPhysicalDeviceXcbPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR>(dlsym(libvulkan, "vkGetPhysicalDeviceXcbPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
        vkCreateWaylandSurfaceKHR = reinterpret_cast<PFN_vkCreateWaylandSurfaceKHR>(dlsym(libvulkan, "vkCreateWaylandSurfaceKHR"));
        vkGetPhysicalDeviceWaylandPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR>(dlsym(libvulkan, "vkGetPhysicalDeviceWaylandPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
        vkCreateMirSurfaceKHR = reinterpret_cast<PFN_vkCreateMirSurfaceKHR>(dlsym(libvulkan, "vkCreateMirSurfaceKHR"));
        vkGetPhysicalDeviceMirPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceMirPresentationSupportKHR>(dlsym(libvulkan, "vkGetPhysicalDeviceMirPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
        vkCreateAndroidSurfaceKHR = reinterpret_cast<PFN_vkCreateAndroidSurfaceKHR>(dlsym(libvulkan, "vkCreateAndroidSurfaceKHR"));
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
        vkCreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(dlsym(libvulkan, "vkCreateWin32SurfaceKHR"));
        vkGetPhysicalDeviceWin32PresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR>(dlsym(libvulkan, "vkGetPhysicalDeviceWin32PresentationSupportKHR"));
#endif
#ifdef USE_DEBUG_EXTENTIONS
        vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(dlsym(libvulkan, "vkCreateDebugReportCallbackEXT"));
        vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(dlsym(libvulkan, "vkDestroyDebugReportCallbackEXT"));
        vkDebugReportMessageEXT = reinterpret_cast<PFN_vkDebugReportMessageEXT>(dlsym(libvulkan, "vkDebugReportMessageEXT"));
#endif

        return true;
    }

    PFN_vkCreateInstance vkCreateInstance = nullptr;
    PFN_vkDestroyInstance vkDestroyInstance = nullptr;
    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = nullptr;
    PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures = nullptr;
    PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties = nullptr;
    PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties = nullptr;
    PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = nullptr;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
    PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties = nullptr;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;
    PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = nullptr;
    PFN_vkCreateDevice vkCreateDevice = nullptr;
    PFN_vkDestroyDevice vkDestroyDevice = nullptr;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = nullptr;
    PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties = nullptr;
    PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = nullptr;
    PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties = nullptr;
    PFN_vkGetDeviceQueue vkGetDeviceQueue = nullptr;
    PFN_vkQueueSubmit vkQueueSubmit = nullptr;
    PFN_vkQueueWaitIdle vkQueueWaitIdle = nullptr;
    PFN_vkDeviceWaitIdle vkDeviceWaitIdle = nullptr;
    PFN_vkAllocateMemory vkAllocateMemory = nullptr;
    PFN_vkFreeMemory vkFreeMemory = nullptr;
    PFN_vkMapMemory vkMapMemory = nullptr;
    PFN_vkUnmapMemory vkUnmapMemory = nullptr;
    PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges = nullptr;
    PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges = nullptr;
    PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment = nullptr;
    PFN_vkBindBufferMemory vkBindBufferMemory = nullptr;
    PFN_vkBindImageMemory vkBindImageMemory = nullptr;
    PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements = nullptr;
    PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements = nullptr;
    PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements = nullptr;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties = nullptr;
    PFN_vkQueueBindSparse vkQueueBindSparse = nullptr;
    PFN_vkCreateFence vkCreateFence = nullptr;
    PFN_vkDestroyFence vkDestroyFence = nullptr;
    PFN_vkResetFences vkResetFences = nullptr;
    PFN_vkGetFenceStatus vkGetFenceStatus = nullptr;
    PFN_vkWaitForFences vkWaitForFences = nullptr;
    PFN_vkCreateSemaphore vkCreateSemaphore = nullptr;
    PFN_vkDestroySemaphore vkDestroySemaphore = nullptr;
    PFN_vkCreateEvent vkCreateEvent = nullptr;
    PFN_vkDestroyEvent vkDestroyEvent = nullptr;
    PFN_vkGetEventStatus vkGetEventStatus = nullptr;
    PFN_vkSetEvent vkSetEvent = nullptr;
    PFN_vkResetEvent vkResetEvent = nullptr;
    PFN_vkCreateQueryPool vkCreateQueryPool = nullptr;
    PFN_vkDestroyQueryPool vkDestroyQueryPool = nullptr;
    PFN_vkGetQueryPoolResults vkGetQueryPoolResults = nullptr;
    PFN_vkCreateBuffer vkCreateBuffer = nullptr;
    PFN_vkDestroyBuffer vkDestroyBuffer = nullptr;
    PFN_vkCreateBufferView vkCreateBufferView = nullptr;
    PFN_vkDestroyBufferView vkDestroyBufferView = nullptr;
    PFN_vkCreateImage vkCreateImage = nullptr;
    PFN_vkDestroyImage vkDestroyImage = nullptr;
    PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout = nullptr;
    PFN_vkCreateImageView vkCreateImageView = nullptr;
    PFN_vkDestroyImageView vkDestroyImageView = nullptr;
    PFN_vkCreateShaderModule vkCreateShaderModule = nullptr;
    PFN_vkDestroyShaderModule vkDestroyShaderModule = nullptr;
    PFN_vkCreatePipelineCache vkCreatePipelineCache = nullptr;
    PFN_vkDestroyPipelineCache vkDestroyPipelineCache = nullptr;
    PFN_vkGetPipelineCacheData vkGetPipelineCacheData = nullptr;
    PFN_vkMergePipelineCaches vkMergePipelineCaches = nullptr;
    PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines = nullptr;
    PFN_vkCreateComputePipelines vkCreateComputePipelines = nullptr;
    PFN_vkDestroyPipeline vkDestroyPipeline = nullptr;
    PFN_vkCreatePipelineLayout vkCreatePipelineLayout = nullptr;
    PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout = nullptr;
    PFN_vkCreateSampler vkCreateSampler = nullptr;
    PFN_vkDestroySampler vkDestroySampler = nullptr;
    PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout = nullptr;
    PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout = nullptr;
    PFN_vkCreateDescriptorPool vkCreateDescriptorPool = nullptr;
    PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool = nullptr;
    PFN_vkResetDescriptorPool vkResetDescriptorPool = nullptr;
    PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets = nullptr;
    PFN_vkFreeDescriptorSets vkFreeDescriptorSets = nullptr;
    PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets = nullptr;
    PFN_vkCreateFramebuffer vkCreateFramebuffer = nullptr;
    PFN_vkDestroyFramebuffer vkDestroyFramebuffer = nullptr;
    PFN_vkCreateRenderPass vkCreateRenderPass = nullptr;
    PFN_vkDestroyRenderPass vkDestroyRenderPass = nullptr;
    PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity = nullptr;
    PFN_vkCreateCommandPool vkCreateCommandPool = nullptr;
    PFN_vkDestroyCommandPool vkDestroyCommandPool = nullptr;
    PFN_vkResetCommandPool vkResetCommandPool = nullptr;
    PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers = nullptr;
    PFN_vkFreeCommandBuffers vkFreeCommandBuffers = nullptr;
    PFN_vkBeginCommandBuffer vkBeginCommandBuffer = nullptr;
    PFN_vkEndCommandBuffer vkEndCommandBuffer = nullptr;
    PFN_vkResetCommandBuffer vkResetCommandBuffer = nullptr;
    PFN_vkCmdBindPipeline vkCmdBindPipeline = nullptr;
    PFN_vkCmdSetViewport vkCmdSetViewport = nullptr;
    PFN_vkCmdSetScissor vkCmdSetScissor = nullptr;
    PFN_vkCmdSetLineWidth vkCmdSetLineWidth = nullptr;
    PFN_vkCmdSetDepthBias vkCmdSetDepthBias = nullptr;
    PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants = nullptr;
    PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds = nullptr;
    PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask = nullptr;
    PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask = nullptr;
    PFN_vkCmdSetStencilReference vkCmdSetStencilReference = nullptr;
    PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets = nullptr;
    PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer = nullptr;
    PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers = nullptr;
    PFN_vkCmdDraw vkCmdDraw = nullptr;
    PFN_vkCmdDrawIndexed vkCmdDrawIndexed = nullptr;
    PFN_vkCmdDrawIndirect vkCmdDrawIndirect = nullptr;
    PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect = nullptr;
    PFN_vkCmdDispatch vkCmdDispatch = nullptr;
    PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect = nullptr;
    PFN_vkCmdCopyBuffer vkCmdCopyBuffer = nullptr;
    PFN_vkCmdCopyImage vkCmdCopyImage = nullptr;
    PFN_vkCmdBlitImage vkCmdBlitImage = nullptr;
    PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage = nullptr;
    PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer = nullptr;
    PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer = nullptr;
    PFN_vkCmdFillBuffer vkCmdFillBuffer = nullptr;
    PFN_vkCmdClearColorImage vkCmdClearColorImage = nullptr;
    PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage = nullptr;
    PFN_vkCmdClearAttachments vkCmdClearAttachments = nullptr;
    PFN_vkCmdResolveImage vkCmdResolveImage = nullptr;
    PFN_vkCmdSetEvent vkCmdSetEvent = nullptr;
    PFN_vkCmdResetEvent vkCmdResetEvent = nullptr;
    PFN_vkCmdWaitEvents vkCmdWaitEvents = nullptr;
    PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier = nullptr;
    PFN_vkCmdBeginQuery vkCmdBeginQuery = nullptr;
    PFN_vkCmdEndQuery vkCmdEndQuery = nullptr;
    PFN_vkCmdResetQueryPool vkCmdResetQueryPool = nullptr;
    PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp = nullptr;
    PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults = nullptr;
    PFN_vkCmdPushConstants vkCmdPushConstants = nullptr;
    PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass = nullptr;
    PFN_vkCmdNextSubpass vkCmdNextSubpass = nullptr;
    PFN_vkCmdEndRenderPass vkCmdEndRenderPass = nullptr;
    PFN_vkCmdExecuteCommands vkCmdExecuteCommands = nullptr;
    PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = nullptr;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = nullptr;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = nullptr;
    PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR = nullptr;
    PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR = nullptr;
    PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR = nullptr;
    PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR = nullptr;
    PFN_vkQueuePresentKHR vkQueuePresentKHR = nullptr;
    PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR = nullptr;
    PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR = nullptr;
    PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR = nullptr;
    PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR = nullptr;
    PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR = nullptr;
    PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR = nullptr;
    PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR = nullptr;
    PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR = nullptr;

#ifdef VK_USE_PLATFORM_XLIB_KHR
    PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR = nullptr;
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
    PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR = nullptr;
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR = nullptr;
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
    PFN_vkCreateMirSurfaceKHR vkCreateMirSurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceMirPresentationSupportKHR vkGetPhysicalDeviceMirPresentationSupportKHR = nullptr;
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
    PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR = nullptr;
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
    PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR = nullptr;
#endif
    PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = nullptr;
    PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = nullptr;
    PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT = nullptr;
}
#endif