#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#else
#define VK_USE_PLATFORM_XLIB_KHR
#endif

#include <vulkan/vulkan.h>

// Remove defines from XLib.h (included by vulkan.h) that cause conflicts
#ifndef _WIN32
#undef None
#undef Status
#undef Bool
#undef Always
#endif

#ifdef _WIN32
#pragma comment(lib, "vulkan-1.lib")
#endif

#include "VkHandle.h"
#include "VkResource.h"

namespace ZHU
{
	using WindowHandle = void*;
	using DeviceHandle = VkRenderDevice::SharedPtr;
	using DeviceObjectHandle = VkHandleBase::SharedPtr;

	using HeapGPUHandle = void*;
	using HeapCPUHandle = void*;
	using CommandListHandle = VkHandle<VkCommandBuffer>::SharedPtr;
	using CommandQueueHandle = VkHandle<VkQueue>::SharedPtr;
	using CommandAllocatorHandle = VkHandle<VkCommandPool>::SharedPtr;
	using CommandSignatureHandle = void*;
	using FenceHandle = VkHandle<VkSemaphore>::SharedPtr;
	using ResourceHandle = VkResource<VkBuffer, VkImage>::SharedPtr;
	using RTVHandle = VkResource<VkBufferView, VkImageView>::SharedPtr;
	using DSVHandle = VkResource<VkBufferView, VkImageView>::SharedPtr;
	using SRVHandle = VkResource<VkBufferView, VkImageView>::SharedPtr;
	using UAVHandle = VkResource<VkBufferView, VkImageView>::SharedPtr;
	using CBVHandle = VkResource<VkBufferView, VkImageView>::SharedPtr;
	using FBOHandle = VkFBO::SharedPtr;
	using SamplerHandle = VkHandle<VkSampler>::SharedPtr;
	using GPUAddress = size_t;
	using DescriptorSetHandle = VkHandle<VkDescriptorSet>::SharedPtr;
	using QueryHeapHandle = VkHandle<VkQueryPool>::SharedPtr;

	using GraphicsStateHandle = VkHandle<VkPipeline>::SharedPtr;
	using ComputeStateHandle = VkHandle<VkPipeline>::SharedPtr;
	using ShaderHandle = VkHandle<VkShaderModule>::SharedPtr;
	using ShaderReflectionHandle = void*;
	using RootSignatureHandle = VkRootSignature::SharedPtr;
	using DescriptorHeapHandle = VkHandle<VkDescriptorPool>::SharedPtr;

	using VAOHandle = void*;
	using GPUProgramHandle = void*;
	using DepthStencilStateHandle = void*;
	using RasterizerStateHandle = void*;
	using BlendStateHandle = void*;
}