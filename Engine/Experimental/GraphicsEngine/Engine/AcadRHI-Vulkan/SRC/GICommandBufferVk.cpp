#include "GICommandBufferVk.h"
#include "GICommandPoolVk.h"
#include "GIDeviceQueueVk.h"
#include "GIDeviceVk.h"
#include "GIRenderPassVk.h"

namespace AutoCAD::Graphics::Engine
{
    GICommandBufferVk::GICommandBufferVk(SharedPtr<GICommandPoolVk> pool, bool secondary)
        : GIWeakDeviceObjectVk(pool->GetDevice()->weak_from_this())
        , mCommandPool(*pool)
        , mQueue(pool->GetQueue())
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.commandPool = *pool;
        allocInfo.level = secondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        VK_CHECK(vkAllocateCommandBuffers(*pool->GetDevice(), &allocInfo, &mCommandBuffer));
        assert(mCommandBuffer != VK_NULL_HANDLE);
    }

    GICommandBufferVk::~GICommandBufferVk()
    {
        VkDevice device = *mDevice.lock();
        vkFreeCommandBuffers(device, mCommandPool, 1, &mCommandBuffer);
        mCommandBuffer = VK_NULL_HANDLE;
    }

    bool GICommandBufferVk::IsValid() const
    {
        return mCommandBuffer != VK_NULL_HANDLE;
    }

    void GICommandBufferVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mCommandBuffer, 
            VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT,
            name);
    }

    void GICommandBufferVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mCommandBuffer,
            VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT,
            tag);
    }

    GICommandBufferVk::operator const VkCommandBuffer& () const
    {
        return mCommandBuffer;
    }

    GICommandBufferVk::EState GICommandBufferVk::GetState() const
    {
        return mState;
    }

    VkCommandPool GICommandBufferVk::GetCommandPool() const
    {
        return mCommandPool;
    }

    VkQueue GICommandBufferVk::GetQueue() const
    {
        return mQueue;
    }

    void GICommandBufferVk::AddSemaphoreToWait(VkSemaphore waitSemaphore)
    {
        mSemaphoresToWait.push_back(waitSemaphore);
    }

    void GICommandBufferVk::SetStageFlagToWait(VkPipelineStageFlags waitStageFlag)
    {
        mStageFlagsToWait = waitStageFlag;
    }

    void GICommandBufferVk::AddSemaphoreToSignal(VkSemaphore signalSemaphore)
    {
        mSemaphoresToSignal.push_back(signalSemaphore);
    }

    void GICommandBufferVk::SetFenceToSignal(VkFence signalFence)
    {
        mFenceToSignal = signalFence;
    }

    void GICommandBufferVk::Begin()
    {}

    void GICommandBufferVk::BeginRenderPass(SharedPtr<GIRenderPassVk> renderPass)
    {}

    void GICommandBufferVk::EndRenderPass()
    {}

    void GICommandBufferVk::End()
    {}

    void GICommandBufferVk::Submit()
    {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        submitInfo.waitSemaphoreCount = (uint32_t)mSemaphoresToWait.size();
        submitInfo.pWaitSemaphores = mSemaphoresToWait.empty() ? nullptr : mSemaphoresToWait.data();
        submitInfo.pWaitDstStageMask = mStageFlagsToWait ? &(mStageFlagsToWait.value()) : nullptr;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &mCommandBuffer;
        submitInfo.signalSemaphoreCount = (uint32_t)mSemaphoresToSignal.size();
        submitInfo.pSignalSemaphores = mSemaphoresToSignal.empty() ? nullptr : mSemaphoresToSignal.data();
        
        if (mFenceToSignal)
        {
            VkDevice device = *mDevice.lock();
            VK_CHECK(vkResetFences(device, 1, &mFenceToSignal));
        }

        VK_CHECK(vkQueueSubmit(mQueue, 1, &submitInfo, mFenceToSignal));
    }

    void GICommandBufferVk::SubmitIdle()
    {
        Submit();

        VkDevice device = *mDevice.lock();
        VK_CHECK(vkWaitForFences(device, 1, &mFenceToSignal, VK_TRUE, UINT64_MAX));
        VK_CHECK(vkResetFences(device, 1, &mFenceToSignal));
    }
}