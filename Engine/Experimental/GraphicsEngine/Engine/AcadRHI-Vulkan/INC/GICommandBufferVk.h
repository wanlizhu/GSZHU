#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GICommandPoolVk;
    class GIRenderPassVk;

    class GICommandBufferVk : public GIWeakDeviceObjectVk
    {
        friend class GICommandPoolVk;
        DECL_WEAK_DEVICE_OBJECT(GICommandBufferVk)
    public:
        enum class EState
        {
            NotAllocated,
            ReadyToBegin,
            InsideBegin,
            InsideRenderPass,
            RecordEnded,
            Submitted,
        };

        virtual ~GICommandBufferVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkCommandBuffer& () const;
        VkCommandPool GetCommandPool() const;
        VkQueue GetQueue() const;
        EState GetState() const;
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
        GICommandBufferVk(SharedPtr<GICommandPoolVk> pool, bool secondary = false);
        
    private:
        VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;
        VkCommandPool mCommandPool = VK_NULL_HANDLE;
        VkQueue mQueue = VK_NULL_HANDLE;
        EState mState = EState::NotAllocated;

        std::vector<VkSemaphore> mSemaphoresToWait;
        std::optional<VkPipelineStageFlags> mStageFlagsToWait;
        std::vector<VkSemaphore> mSemaphoresToSignal;
        VkFence mFenceToSignal = VK_NULL_HANDLE;
    };
}