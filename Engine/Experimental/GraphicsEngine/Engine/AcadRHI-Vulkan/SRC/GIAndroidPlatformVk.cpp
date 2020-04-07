#if defined(__ANDROID__)

#include "GIAndroidPlatformVk.h"
#include "GIPlatformVk.h"
#include <dlfcn.h>

namespace AutoCAD::Graphics::Engine
{
    GIAndroidPlatformVk& GIAndroidPlatformVk::Get()
    {
        static GIAndroidPlatformVk instance;
        return instance;
    }

    void GIAndroidPlatformVk::LoadVulkanLibrary()
    {
        // Load vulkan library
        m_libvulkan = dlopen("m_libvulkan.so", RTLD_NOW | RTLD_LOCAL);
        if (!m_libvulkan)
        {
            LOG_ERROR("Could not load vulkan library : %s!\n", dlerror());
            return false;
        }

        LoadVulkanFunctions();
        return true;
    }

    void GIAndroidPlatformVk::FreeVulkanLibrary()
    {
        if (m_libvulkan)
        {
            flclose(m_libvulkan);
            m_libvulkan = nullptr;
        }
    }

    std::vector<const char*> GIAndroidPlatformVk::RequiredInstanceLayers()
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

    std::vector<const char*> GIAndroidPlatformVk::RequiredInstanceExtensions()
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

    std::vector<const char*> GIAndroidPlatformVk::RequiredDeviceExtensions(EGPUVendorID vendorId)
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

    VkPhysicalDeviceFeatures GIAndroidPlatformVk::RequiredDeviceFeatures()
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
        vkCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(dlsym(m_libvulkan, "vkCreateInstance"));
        vkDestroyInstance = reinterpret_cast<PFN_vkDestroyInstance>(dlsym(m_libvulkan, "vkDestroyInstance"));
        vkEnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(dlsym(m_libvulkan, "vkEnumeratePhysicalDevices"));
        vkGetPhysicalDeviceFeatures = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures>(dlsym(m_libvulkan, "vkGetPhysicalDeviceFeatures"));
        vkGetPhysicalDeviceFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceFormatProperties>(dlsym(m_libvulkan, "vkGetPhysicalDeviceFormatProperties"));
        vkGetPhysicalDeviceImageFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceImageFormatProperties>(dlsym(m_libvulkan, "vkGetPhysicalDeviceImageFormatProperties"));
        vkGetPhysicalDeviceProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties>(dlsym(m_libvulkan, "vkGetPhysicalDeviceProperties"));
        vkGetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(dlsym(m_libvulkan, "vkGetPhysicalDeviceQueueFamilyProperties"));
        vkGetPhysicalDeviceMemoryProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceMemoryProperties>(dlsym(m_libvulkan, "vkGetPhysicalDeviceMemoryProperties"));
        vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(dlsym(m_libvulkan, "vkGetInstanceProcAddr"));
        vkGetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(dlsym(m_libvulkan, "vkGetDeviceProcAddr"));
        vkCreateDevice = reinterpret_cast<PFN_vkCreateDevice>(dlsym(m_libvulkan, "vkCreateDevice"));
        vkDestroyDevice = reinterpret_cast<PFN_vkDestroyDevice>(dlsym(m_libvulkan, "vkDestroyDevice"));
        vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(dlsym(m_libvulkan, "vkEnumerateInstanceExtensionProperties"));
        vkEnumerateDeviceExtensionProperties = reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(dlsym(m_libvulkan, "vkEnumerateDeviceExtensionProperties"));
        vkEnumerateInstanceLayerProperties = reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(dlsym(m_libvulkan, "vkEnumerateInstanceLayerProperties"));
        vkEnumerateDeviceLayerProperties = reinterpret_cast<PFN_vkEnumerateDeviceLayerProperties>(dlsym(m_libvulkan, "vkEnumerateDeviceLayerProperties"));
        vkGetDeviceQueue = reinterpret_cast<PFN_vkGetDeviceQueue>(dlsym(m_libvulkan, "vkGetDeviceQueue"));
        vkQueueSubmit = reinterpret_cast<PFN_vkQueueSubmit>(dlsym(m_libvulkan, "vkQueueSubmit"));
        vkQueueWaitIdle = reinterpret_cast<PFN_vkQueueWaitIdle>(dlsym(m_libvulkan, "vkQueueWaitIdle"));
        vkDeviceWaitIdle = reinterpret_cast<PFN_vkDeviceWaitIdle>(dlsym(m_libvulkan, "vkDeviceWaitIdle"));
        vkAllocateMemory = reinterpret_cast<PFN_vkAllocateMemory>(dlsym(m_libvulkan, "vkAllocateMemory"));
        vkFreeMemory = reinterpret_cast<PFN_vkFreeMemory>(dlsym(m_libvulkan, "vkFreeMemory"));
        vkMapMemory = reinterpret_cast<PFN_vkMapMemory>(dlsym(m_libvulkan, "vkMapMemory"));
        vkUnmapMemory = reinterpret_cast<PFN_vkUnmapMemory>(dlsym(m_libvulkan, "vkUnmapMemory"));
        vkFlushMappedMemoryRanges = reinterpret_cast<PFN_vkFlushMappedMemoryRanges>(dlsym(m_libvulkan, "vkFlushMappedMemoryRanges"));
        vkInvalidateMappedMemoryRanges = reinterpret_cast<PFN_vkInvalidateMappedMemoryRanges>(dlsym(m_libvulkan, "vkInvalidateMappedMemoryRanges"));
        vkGetDeviceMemoryCommitment = reinterpret_cast<PFN_vkGetDeviceMemoryCommitment>(dlsym(m_libvulkan, "vkGetDeviceMemoryCommitment"));
        vkBindBufferMemory = reinterpret_cast<PFN_vkBindBufferMemory>(dlsym(m_libvulkan, "vkBindBufferMemory"));
        vkBindImageMemory = reinterpret_cast<PFN_vkBindImageMemory>(dlsym(m_libvulkan, "vkBindImageMemory"));
        vkGetBufferMemoryRequirements = reinterpret_cast<PFN_vkGetBufferMemoryRequirements>(dlsym(m_libvulkan, "vkGetBufferMemoryRequirements"));
        vkGetImageMemoryRequirements = reinterpret_cast<PFN_vkGetImageMemoryRequirements>(dlsym(m_libvulkan, "vkGetImageMemoryRequirements"));
        vkGetImageSparseMemoryRequirements = reinterpret_cast<PFN_vkGetImageSparseMemoryRequirements>(dlsym(m_libvulkan, "vkGetImageSparseMemoryRequirements"));
        vkGetPhysicalDeviceSparseImageFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceSparseImageFormatProperties>(dlsym(m_libvulkan, "vkGetPhysicalDeviceSparseImageFormatProperties"));
        vkQueueBindSparse = reinterpret_cast<PFN_vkQueueBindSparse>(dlsym(m_libvulkan, "vkQueueBindSparse"));
        vkCreateFence = reinterpret_cast<PFN_vkCreateFence>(dlsym(m_libvulkan, "vkCreateFence"));
        vkDestroyFence = reinterpret_cast<PFN_vkDestroyFence>(dlsym(m_libvulkan, "vkDestroyFence"));
        vkResetFences = reinterpret_cast<PFN_vkResetFences>(dlsym(m_libvulkan, "vkResetFences"));
        vkGetFenceStatus = reinterpret_cast<PFN_vkGetFenceStatus>(dlsym(m_libvulkan, "vkGetFenceStatus"));
        vkWaitForFences = reinterpret_cast<PFN_vkWaitForFences>(dlsym(m_libvulkan, "vkWaitForFences"));
        vkCreateSemaphore = reinterpret_cast<PFN_vkCreateSemaphore>(dlsym(m_libvulkan, "vkCreateSemaphore"));
        vkDestroySemaphore = reinterpret_cast<PFN_vkDestroySemaphore>(dlsym(m_libvulkan, "vkDestroySemaphore"));
        vkCreateEvent = reinterpret_cast<PFN_vkCreateEvent>(dlsym(m_libvulkan, "vkCreateEvent"));
        vkDestroyEvent = reinterpret_cast<PFN_vkDestroyEvent>(dlsym(m_libvulkan, "vkDestroyEvent"));
        vkGetEventStatus = reinterpret_cast<PFN_vkGetEventStatus>(dlsym(m_libvulkan, "vkGetEventStatus"));
        vkSetEvent = reinterpret_cast<PFN_vkSetEvent>(dlsym(m_libvulkan, "vkSetEvent"));
        vkResetEvent = reinterpret_cast<PFN_vkResetEvent>(dlsym(m_libvulkan, "vkResetEvent"));
        vkCreateQueryPool = reinterpret_cast<PFN_vkCreateQueryPool>(dlsym(m_libvulkan, "vkCreateQueryPool"));
        vkDestroyQueryPool = reinterpret_cast<PFN_vkDestroyQueryPool>(dlsym(m_libvulkan, "vkDestroyQueryPool"));
        vkGetQueryPoolResults = reinterpret_cast<PFN_vkGetQueryPoolResults>(dlsym(m_libvulkan, "vkGetQueryPoolResults"));
        vkCreateBuffer = reinterpret_cast<PFN_vkCreateBuffer>(dlsym(m_libvulkan, "vkCreateBuffer"));
        vkDestroyBuffer = reinterpret_cast<PFN_vkDestroyBuffer>(dlsym(m_libvulkan, "vkDestroyBuffer"));
        vkCreateBufferView = reinterpret_cast<PFN_vkCreateBufferView>(dlsym(m_libvulkan, "vkCreateBufferView"));
        vkDestroyBufferView = reinterpret_cast<PFN_vkDestroyBufferView>(dlsym(m_libvulkan, "vkDestroyBufferView"));
        vkCreateImage = reinterpret_cast<PFN_vkCreateImage>(dlsym(m_libvulkan, "vkCreateImage"));
        vkDestroyImage = reinterpret_cast<PFN_vkDestroyImage>(dlsym(m_libvulkan, "vkDestroyImage"));
        vkGetImageSubresourceLayout = reinterpret_cast<PFN_vkGetImageSubresourceLayout>(dlsym(m_libvulkan, "vkGetImageSubresourceLayout"));
        vkCreateImageView = reinterpret_cast<PFN_vkCreateImageView>(dlsym(m_libvulkan, "vkCreateImageView"));
        vkDestroyImageView = reinterpret_cast<PFN_vkDestroyImageView>(dlsym(m_libvulkan, "vkDestroyImageView"));
        vkCreateShaderModule = reinterpret_cast<PFN_vkCreateShaderModule>(dlsym(m_libvulkan, "vkCreateShaderModule"));
        vkDestroyShaderModule = reinterpret_cast<PFN_vkDestroyShaderModule>(dlsym(m_libvulkan, "vkDestroyShaderModule"));
        vkCreatePipelineCache = reinterpret_cast<PFN_vkCreatePipelineCache>(dlsym(m_libvulkan, "vkCreatePipelineCache"));
        vkDestroyPipelineCache = reinterpret_cast<PFN_vkDestroyPipelineCache>(dlsym(m_libvulkan, "vkDestroyPipelineCache"));
        vkGetPipelineCacheData = reinterpret_cast<PFN_vkGetPipelineCacheData>(dlsym(m_libvulkan, "vkGetPipelineCacheData"));
        vkMergePipelineCaches = reinterpret_cast<PFN_vkMergePipelineCaches>(dlsym(m_libvulkan, "vkMergePipelineCaches"));
        vkCreateGraphicsPipelines = reinterpret_cast<PFN_vkCreateGraphicsPipelines>(dlsym(m_libvulkan, "vkCreateGraphicsPipelines"));
        vkCreateComputePipelines = reinterpret_cast<PFN_vkCreateComputePipelines>(dlsym(m_libvulkan, "vkCreateComputePipelines"));
        vkDestroyPipeline = reinterpret_cast<PFN_vkDestroyPipeline>(dlsym(m_libvulkan, "vkDestroyPipeline"));
        vkCreatePipelineLayout = reinterpret_cast<PFN_vkCreatePipelineLayout>(dlsym(m_libvulkan, "vkCreatePipelineLayout"));
        vkDestroyPipelineLayout = reinterpret_cast<PFN_vkDestroyPipelineLayout>(dlsym(m_libvulkan, "vkDestroyPipelineLayout"));
        vkCreateSampler = reinterpret_cast<PFN_vkCreateSampler>(dlsym(m_libvulkan, "vkCreateSampler"));
        vkDestroySampler = reinterpret_cast<PFN_vkDestroySampler>(dlsym(m_libvulkan, "vkDestroySampler"));
        vkCreateDescriptorSetLayout = reinterpret_cast<PFN_vkCreateDescriptorSetLayout>(dlsym(m_libvulkan, "vkCreateDescriptorSetLayout"));
        vkDestroyDescriptorSetLayout = reinterpret_cast<PFN_vkDestroyDescriptorSetLayout>(dlsym(m_libvulkan, "vkDestroyDescriptorSetLayout"));
        vkCreateDescriptorPool = reinterpret_cast<PFN_vkCreateDescriptorPool>(dlsym(m_libvulkan, "vkCreateDescriptorPool"));
        vkDestroyDescriptorPool = reinterpret_cast<PFN_vkDestroyDescriptorPool>(dlsym(m_libvulkan, "vkDestroyDescriptorPool"));
        vkResetDescriptorPool = reinterpret_cast<PFN_vkResetDescriptorPool>(dlsym(m_libvulkan, "vkResetDescriptorPool"));
        vkAllocateDescriptorSets = reinterpret_cast<PFN_vkAllocateDescriptorSets>(dlsym(m_libvulkan, "vkAllocateDescriptorSets"));
        vkFreeDescriptorSets = reinterpret_cast<PFN_vkFreeDescriptorSets>(dlsym(m_libvulkan, "vkFreeDescriptorSets"));
        vkUpdateDescriptorSets = reinterpret_cast<PFN_vkUpdateDescriptorSets>(dlsym(m_libvulkan, "vkUpdateDescriptorSets"));
        vkCreateFramebuffer = reinterpret_cast<PFN_vkCreateFramebuffer>(dlsym(m_libvulkan, "vkCreateFramebuffer"));
        vkDestroyFramebuffer = reinterpret_cast<PFN_vkDestroyFramebuffer>(dlsym(m_libvulkan, "vkDestroyFramebuffer"));
        vkCreateRenderPass = reinterpret_cast<PFN_vkCreateRenderPass>(dlsym(m_libvulkan, "vkCreateRenderPass"));
        vkDestroyRenderPass = reinterpret_cast<PFN_vkDestroyRenderPass>(dlsym(m_libvulkan, "vkDestroyRenderPass"));
        vkGetRenderAreaGranularity = reinterpret_cast<PFN_vkGetRenderAreaGranularity>(dlsym(m_libvulkan, "vkGetRenderAreaGranularity"));
        vkCreateCommandPool = reinterpret_cast<PFN_vkCreateCommandPool>(dlsym(m_libvulkan, "vkCreateCommandPool"));
        vkDestroyCommandPool = reinterpret_cast<PFN_vkDestroyCommandPool>(dlsym(m_libvulkan, "vkDestroyCommandPool"));
        vkResetCommandPool = reinterpret_cast<PFN_vkResetCommandPool>(dlsym(m_libvulkan, "vkResetCommandPool"));
        vkAllocateCommandBuffers = reinterpret_cast<PFN_vkAllocateCommandBuffers>(dlsym(m_libvulkan, "vkAllocateCommandBuffers"));
        vkFreeCommandBuffers = reinterpret_cast<PFN_vkFreeCommandBuffers>(dlsym(m_libvulkan, "vkFreeCommandBuffers"));
        vkBeginCommandBuffer = reinterpret_cast<PFN_vkBeginCommandBuffer>(dlsym(m_libvulkan, "vkBeginCommandBuffer"));
        vkEndCommandBuffer = reinterpret_cast<PFN_vkEndCommandBuffer>(dlsym(m_libvulkan, "vkEndCommandBuffer"));
        vkResetCommandBuffer = reinterpret_cast<PFN_vkResetCommandBuffer>(dlsym(m_libvulkan, "vkResetCommandBuffer"));
        vkCmdBindPipeline = reinterpret_cast<PFN_vkCmdBindPipeline>(dlsym(m_libvulkan, "vkCmdBindPipeline"));
        vkCmdSetViewport = reinterpret_cast<PFN_vkCmdSetViewport>(dlsym(m_libvulkan, "vkCmdSetViewport"));
        vkCmdSetScissor = reinterpret_cast<PFN_vkCmdSetScissor>(dlsym(m_libvulkan, "vkCmdSetScissor"));
        vkCmdSetLineWidth = reinterpret_cast<PFN_vkCmdSetLineWidth>(dlsym(m_libvulkan, "vkCmdSetLineWidth"));
        vkCmdSetDepthBias = reinterpret_cast<PFN_vkCmdSetDepthBias>(dlsym(m_libvulkan, "vkCmdSetDepthBias"));
        vkCmdSetBlendConstants = reinterpret_cast<PFN_vkCmdSetBlendConstants>(dlsym(m_libvulkan, "vkCmdSetBlendConstants"));
        vkCmdSetDepthBounds = reinterpret_cast<PFN_vkCmdSetDepthBounds>(dlsym(m_libvulkan, "vkCmdSetDepthBounds"));
        vkCmdSetStencilCompareMask = reinterpret_cast<PFN_vkCmdSetStencilCompareMask>(dlsym(m_libvulkan, "vkCmdSetStencilCompareMask"));
        vkCmdSetStencilWriteMask = reinterpret_cast<PFN_vkCmdSetStencilWriteMask>(dlsym(m_libvulkan, "vkCmdSetStencilWriteMask"));
        vkCmdSetStencilReference = reinterpret_cast<PFN_vkCmdSetStencilReference>(dlsym(m_libvulkan, "vkCmdSetStencilReference"));
        vkCmdBindDescriptorSets = reinterpret_cast<PFN_vkCmdBindDescriptorSets>(dlsym(m_libvulkan, "vkCmdBindDescriptorSets"));
        vkCmdBindIndexBuffer = reinterpret_cast<PFN_vkCmdBindIndexBuffer>(dlsym(m_libvulkan, "vkCmdBindIndexBuffer"));
        vkCmdBindVertexBuffers = reinterpret_cast<PFN_vkCmdBindVertexBuffers>(dlsym(m_libvulkan, "vkCmdBindVertexBuffers"));
        vkCmdDraw = reinterpret_cast<PFN_vkCmdDraw>(dlsym(m_libvulkan, "vkCmdDraw"));
        vkCmdDrawIndexed = reinterpret_cast<PFN_vkCmdDrawIndexed>(dlsym(m_libvulkan, "vkCmdDrawIndexed"));
        vkCmdDrawIndirect = reinterpret_cast<PFN_vkCmdDrawIndirect>(dlsym(m_libvulkan, "vkCmdDrawIndirect"));
        vkCmdDrawIndexedIndirect = reinterpret_cast<PFN_vkCmdDrawIndexedIndirect>(dlsym(m_libvulkan, "vkCmdDrawIndexedIndirect"));
        vkCmdDispatch = reinterpret_cast<PFN_vkCmdDispatch>(dlsym(m_libvulkan, "vkCmdDispatch"));
        vkCmdDispatchIndirect = reinterpret_cast<PFN_vkCmdDispatchIndirect>(dlsym(m_libvulkan, "vkCmdDispatchIndirect"));
        vkCmdCopyBuffer = reinterpret_cast<PFN_vkCmdCopyBuffer>(dlsym(m_libvulkan, "vkCmdCopyBuffer"));
        vkCmdCopyImage = reinterpret_cast<PFN_vkCmdCopyImage>(dlsym(m_libvulkan, "vkCmdCopyImage"));
        vkCmdBlitImage = reinterpret_cast<PFN_vkCmdBlitImage>(dlsym(m_libvulkan, "vkCmdBlitImage"));
        vkCmdCopyBufferToImage = reinterpret_cast<PFN_vkCmdCopyBufferToImage>(dlsym(m_libvulkan, "vkCmdCopyBufferToImage"));
        vkCmdCopyImageToBuffer = reinterpret_cast<PFN_vkCmdCopyImageToBuffer>(dlsym(m_libvulkan, "vkCmdCopyImageToBuffer"));
        vkCmdUpdateBuffer = reinterpret_cast<PFN_vkCmdUpdateBuffer>(dlsym(m_libvulkan, "vkCmdUpdateBuffer"));
        vkCmdFillBuffer = reinterpret_cast<PFN_vkCmdFillBuffer>(dlsym(m_libvulkan, "vkCmdFillBuffer"));
        vkCmdClearColorImage = reinterpret_cast<PFN_vkCmdClearColorImage>(dlsym(m_libvulkan, "vkCmdClearColorImage"));
        vkCmdClearDepthStencilImage = reinterpret_cast<PFN_vkCmdClearDepthStencilImage>(dlsym(m_libvulkan, "vkCmdClearDepthStencilImage"));
        vkCmdClearAttachments = reinterpret_cast<PFN_vkCmdClearAttachments>(dlsym(m_libvulkan, "vkCmdClearAttachments"));
        vkCmdResolveImage = reinterpret_cast<PFN_vkCmdResolveImage>(dlsym(m_libvulkan, "vkCmdResolveImage"));
        vkCmdSetEvent = reinterpret_cast<PFN_vkCmdSetEvent>(dlsym(m_libvulkan, "vkCmdSetEvent"));
        vkCmdResetEvent = reinterpret_cast<PFN_vkCmdResetEvent>(dlsym(m_libvulkan, "vkCmdResetEvent"));
        vkCmdWaitEvents = reinterpret_cast<PFN_vkCmdWaitEvents>(dlsym(m_libvulkan, "vkCmdWaitEvents"));
        vkCmdPipelineBarrier = reinterpret_cast<PFN_vkCmdPipelineBarrier>(dlsym(m_libvulkan, "vkCmdPipelineBarrier"));
        vkCmdBeginQuery = reinterpret_cast<PFN_vkCmdBeginQuery>(dlsym(m_libvulkan, "vkCmdBeginQuery"));
        vkCmdEndQuery = reinterpret_cast<PFN_vkCmdEndQuery>(dlsym(m_libvulkan, "vkCmdEndQuery"));
        vkCmdResetQueryPool = reinterpret_cast<PFN_vkCmdResetQueryPool>(dlsym(m_libvulkan, "vkCmdResetQueryPool"));
        vkCmdWriteTimestamp = reinterpret_cast<PFN_vkCmdWriteTimestamp>(dlsym(m_libvulkan, "vkCmdWriteTimestamp"));
        vkCmdCopyQueryPoolResults = reinterpret_cast<PFN_vkCmdCopyQueryPoolResults>(dlsym(m_libvulkan, "vkCmdCopyQueryPoolResults"));
        vkCmdPushConstants = reinterpret_cast<PFN_vkCmdPushConstants>(dlsym(m_libvulkan, "vkCmdPushConstants"));
        vkCmdBeginRenderPass = reinterpret_cast<PFN_vkCmdBeginRenderPass>(dlsym(m_libvulkan, "vkCmdBeginRenderPass"));
        vkCmdNextSubpass = reinterpret_cast<PFN_vkCmdNextSubpass>(dlsym(m_libvulkan, "vkCmdNextSubpass"));
        vkCmdEndRenderPass = reinterpret_cast<PFN_vkCmdEndRenderPass>(dlsym(m_libvulkan, "vkCmdEndRenderPass"));
        vkCmdExecuteCommands = reinterpret_cast<PFN_vkCmdExecuteCommands>(dlsym(m_libvulkan, "vkCmdExecuteCommands"));
        vkDestroySurfaceKHR = reinterpret_cast<PFN_vkDestroySurfaceKHR>(dlsym(m_libvulkan, "vkDestroySurfaceKHR"));
        vkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(dlsym(m_libvulkan, "vkGetPhysicalDeviceSurfaceSupportKHR"));
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(dlsym(m_libvulkan, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
        vkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(dlsym(m_libvulkan, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
        vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(dlsym(m_libvulkan, "vkGetPhysicalDeviceSurfacePresentModesKHR"));
        vkCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(dlsym(m_libvulkan, "vkCreateSwapchainKHR"));
        vkDestroySwapchainKHR = reinterpret_cast<PFN_vkDestroySwapchainKHR>(dlsym(m_libvulkan, "vkDestroySwapchainKHR"));
        vkGetSwapchainImagesKHR = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(dlsym(m_libvulkan, "vkGetSwapchainImagesKHR"));
        vkAcquireNextImageKHR = reinterpret_cast<PFN_vkAcquireNextImageKHR>(dlsym(m_libvulkan, "vkAcquireNextImageKHR"));
        vkQueuePresentKHR = reinterpret_cast<PFN_vkQueuePresentKHR>(dlsym(m_libvulkan, "vkQueuePresentKHR"));
        vkGetPhysicalDeviceDisplayPropertiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceDisplayPropertiesKHR>(dlsym(m_libvulkan, "vkGetPhysicalDeviceDisplayPropertiesKHR"));
        vkGetPhysicalDeviceDisplayPlanePropertiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR>(dlsym(m_libvulkan, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR"));
        vkGetDisplayPlaneSupportedDisplaysKHR = reinterpret_cast<PFN_vkGetDisplayPlaneSupportedDisplaysKHR>(dlsym(m_libvulkan, "vkGetDisplayPlaneSupportedDisplaysKHR"));
        vkGetDisplayModePropertiesKHR = reinterpret_cast<PFN_vkGetDisplayModePropertiesKHR>(dlsym(m_libvulkan, "vkGetDisplayModePropertiesKHR"));
        vkCreateDisplayModeKHR = reinterpret_cast<PFN_vkCreateDisplayModeKHR>(dlsym(m_libvulkan, "vkCreateDisplayModeKHR"));
        vkGetDisplayPlaneCapabilitiesKHR = reinterpret_cast<PFN_vkGetDisplayPlaneCapabilitiesKHR>(dlsym(m_libvulkan, "vkGetDisplayPlaneCapabilitiesKHR"));
        vkCreateDisplayPlaneSurfaceKHR = reinterpret_cast<PFN_vkCreateDisplayPlaneSurfaceKHR>(dlsym(m_libvulkan, "vkCreateDisplayPlaneSurfaceKHR"));
        vkCreateSharedSwapchainsKHR = reinterpret_cast<PFN_vkCreateSharedSwapchainsKHR>(dlsym(m_libvulkan, "vkCreateSharedSwapchainsKHR"));

#ifdef VK_USE_PLATFORM_XLIB_KHR
        vkCreateXlibSurfaceKHR = reinterpret_cast<PFN_vkCreateXlibSurfaceKHR>(dlsym(m_libvulkan, "vkCreateXlibSurfaceKHR"));
        vkGetPhysicalDeviceXlibPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR>(dlsym(m_libvulkan, "vkGetPhysicalDeviceXlibPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
        vkCreateXcbSurfaceKHR = reinterpret_cast<PFN_vkCreateXcbSurfaceKHR>(dlsym(m_libvulkan, "vkCreateXcbSurfaceKHR"));
        vkGetPhysicalDeviceXcbPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR>(dlsym(m_libvulkan, "vkGetPhysicalDeviceXcbPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
        vkCreateWaylandSurfaceKHR = reinterpret_cast<PFN_vkCreateWaylandSurfaceKHR>(dlsym(m_libvulkan, "vkCreateWaylandSurfaceKHR"));
        vkGetPhysicalDeviceWaylandPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR>(dlsym(m_libvulkan, "vkGetPhysicalDeviceWaylandPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
        vkCreateMirSurfaceKHR = reinterpret_cast<PFN_vkCreateMirSurfaceKHR>(dlsym(m_libvulkan, "vkCreateMirSurfaceKHR"));
        vkGetPhysicalDeviceMirPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceMirPresentationSupportKHR>(dlsym(m_libvulkan, "vkGetPhysicalDeviceMirPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
        vkCreateAndroidSurfaceKHR = reinterpret_cast<PFN_vkCreateAndroidSurfaceKHR>(dlsym(m_libvulkan, "vkCreateAndroidSurfaceKHR"));
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
        vkCreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(dlsym(m_libvulkan, "vkCreateWin32SurfaceKHR"));
        vkGetPhysicalDeviceWin32PresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR>(dlsym(m_libvulkan, "vkGetPhysicalDeviceWin32PresentationSupportKHR"));
#endif
#ifdef USE_DEBUG_EXTENTIONS
        vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(dlsym(m_libvulkan, "vkCreateDebugReportCallbackEXT"));
        vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(dlsym(m_libvulkan, "vkDestroyDebugReportCallbackEXT"));
        vkDebugReportMessageEXT = reinterpret_cast<PFN_vkDebugReportMessageEXT>(dlsym(m_libvulkan, "vkDebugReportMessageEXT"));
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