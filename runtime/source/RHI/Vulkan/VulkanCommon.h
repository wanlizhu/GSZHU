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
            return mFuncPtr != nullptr;
        }

        template<typename... ARGS>
        VkResult operator()(ARGS&& ... args)
        {
            return call<VkResult>(std::forward<ARGS>(args)...);
        }

        template<typename RT, typename... ARGS>
        RT call(ARGS&& ... args)
        {
            assert(mFuncPtr != nullptr);
            return mFuncPtr(std::forward<ARGS>(args)...);
        }

    private:
        _FuncPtr_  mFuncPtr = nullptr;
    };
}