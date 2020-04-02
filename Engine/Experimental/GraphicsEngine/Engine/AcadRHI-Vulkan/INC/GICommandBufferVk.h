#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GICommandPoolVk;
    class GIRenderPassVk;

    enum class ECommandBufferState
    {
        NotAllocated,
        ReadyToBegin,
        InsideBegin,
        InsideRenderPass,
        RecordEnded,
        Submitted,
    };

    class GICommandBufferVk : public GIWeakDeviceObjectVk
    {
        friend class GICommandPoolVk;
        DECL_WEAK_DEVICE_OBJECT(GICommandBufferVk)
    public:
        virtual ~GICommandBufferVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkCommandBuffer& () const;
        ECommandBufferState GetState() const;
        VkCommandPool GetCommandPool() const;
        VkQueue GetQueue() const;
        void AddPendingCommandCount();
        void SetCommandThreshold(uint32_t threshold);

        void AddSemaphoreToWait(VkSemaphore waitSemaphore);
        void SetStageFlagToWait(VkPipelineStageFlags waitStageFlag);
        void AddSemaphoreToSignal(VkSemaphore signalSemaphore);
        void SetFenceToSignal(VkFence signalFence);
        
        void Begin();
        void BeginRenderPass(SharedPtr<GIRenderPassVk> renderPass);
        void EndRenderPass();
        void End();

        void Submit();
        void SubmitIdle();

    protected:
        GICommandBufferVk(SharedPtr<GICommandPoolVk> pool, bool secondary);
        
    private:
        VkCommandBuffer mCommandBufferHandle = VK_NULL_HANDLE;
        VkCommandPool mCommandPoolHandle = VK_NULL_HANDLE;
        VkQueue mQueue = VK_NULL_HANDLE;
        ECommandBufferState mState = ECommandBufferState::NotAllocated;
        uint32_t mCommandThreshold = UINT_MAX;
        uint32_t mPendingCommandCount = 0;

        std::vector<VkSemaphore> mSemaphoresToWait;
        std::optional<VkPipelineStageFlags> mStageFlagsToWait;
        std::vector<VkSemaphore> mSemaphoresToSignal;
        VkFence mFenceToSignal = VK_NULL_HANDLE;
    };
}