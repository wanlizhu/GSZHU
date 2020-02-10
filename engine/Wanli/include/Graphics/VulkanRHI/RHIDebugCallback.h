#pragma once

#include "RHIUtilities.h"

namespace Wanli
{
    class RHIDebugCallback
    {
    public:
        enum EReportType
        {
            RT_None = 0,
            RT_Error = 1,
            RT_Warning = (1 << 1),
            RT_PerformanceWarning = (1 << 2),
            RT_Information = (1 << 3),
            RT_Debug = (1 << 4),
            RT_All = RT_Error | RT_Warning | RT_PerformanceWarning | RT_Information | RT_Debug,
        };

        RHIDebugCallback(VkInstance instance);
        RHIDebugCallback(const RHIDebugCallback&) = delete;
        RHIDebugCallback& operator=(const RHIDebugCallback&) = delete;
        virtual ~RHIDebugCallback();

        void Initialize(EReportType reportTypes = RT_All);
        void Destroy();
        bool IsEnabled() const;

    protected:
        VkDebugReportFlagsEXT ToDebugReportFlags(RHIDebugCallback::EReportType types);

    private:
        VkInstance mVkInstance = VK_NULL_HANDLE;
        VkDebugReportCallbackEXT mDebugCallback = VK_NULL_HANDLE;
    };
}