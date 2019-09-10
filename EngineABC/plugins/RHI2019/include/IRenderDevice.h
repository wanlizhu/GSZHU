#pragma once

#include "Config.h"
#include <memory>
#include <vector>
#include <string>

namespace ZHU
{
	struct RenderDeviceDesc
	{
		EResourceFormat mColorFormat = EResourceFormat::RGBA8UnormSrgb;
		EResourceFormat mDepthFormat = EResourceFormat::D32Float;
		uint32_t mAPIVersionMajor = 0;
		uint32_t mAPIVersionMinor = 0;
		std::vector<std::string> mRequiredExtensions;

		bool mEnableVSync = false;
		bool mEnableDebugLayer = false;
		bool mEnableVR = false;

		uint32_t mCommandQueueCounts[ECommandQueue] = { 1, 0, 0 };
#ifdef RHI_USE_D3D12
		std::vector<UUID> mExperimentalFeatures;
#endif
	};

	enum class ERenderDeviceFeature
	{
		None = 0,
		OneSamplePositionToBeSet = 1,   // On D3D12, this means tier 1 support.
		FullSamplePositionsToBeSet = 2, // On D3D12, this means tier 2 support.
		Raytracing = 4,
	};

	class RenderDevice final : public std::enable_shared_from_this<RenderDevice>
	{
	public:
		using SharedPtr = std::shared_ptr<RenderDevice>;
		using SharedConstPtr = std::shared_ptr<const RenderDevice>;
		using NativeHandle = DeviceHandle;

		static SharedPtr Create(WindowHandle window, const RenderDeviceDesc& desc);

		~RenderDevice();
		void SetVSync(bool enable);
		bool IsVSyncOn() const;
		bool IsWindowOccluded() const;
		bool IsFeatureSupported(const std::string& name) const;
		Framebuffer* GetCurrentFramebuffer() const;
		RenderContext* GetRenderContext() const;
		CommandQueue* GetCommandQueue(ECommandQueueType type, uint32_t index) const;
		DeviceHandle GetDeviceHandle() const;


		void Present();
		void Flush();
		void FlushAndSync();
		void ResizeFramebuffer();


	private:
		DeviceHandle mDeviceHandle;
		FenceHandle mFrameFence;
		std::vector<CommandQueue::SharedPtr> mCommandQueues[ECommandQueueType::Count];
		ERenderDeviceFeature mSupportedFeatures = ERenderDeviceFeature::None;

		RenderContext::SharedPtr mRenderContext;
		ResourceAllocator::SharedPtr mResourceAllocator;
		DescriptorPool::SharedPtr mCPUDescriptorPool;
		DescriptorPool::SharedPtr mGPUDescriptorPool;

		WindowHandle mWindowHandle;
		bool mVSyncOn;
		std::vector<Framebuffer::SharedPtr> mFramebuffers;
		uint32_t mCurrentBackbuffer = 0;
	};
}