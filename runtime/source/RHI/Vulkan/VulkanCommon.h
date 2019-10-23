#pragma once

#include <vulkan/vulkan.h>

#define INSTANCE_PROC(instance, proc) GE2::RHI::InstanceProc<PFN_##proc>(instance, ###proc)

namespace GE2::RHI
{
    template<typename _FuncPtr_>
    class InstanceProc
    {
    public:
        InstanceProc(VkInstance instance, const char* proc)
        {
            mFuncPtr = (_FuncPtr_)vkGetInstanceProcAddr(instance, proc);
        }

        inline operator bool() const noexcept
        {
            return mFuncPtr != nullptr && mInstance != VK_NULL_HANDLE;
        }

        template<typename... ARGS>
        VkResult operator()(ARGS&& ... args)
        {
            if (mFuncPtr == nullptr)
                return VK_ERROR_EXTENSION_NOT_PRESENT;

            return mFuncPtr(std::forward<ARGS>(args)...);
        }

    private:
        _FuncPtr_  mFuncPtr = nullptr;
    };
}