#pragma once

#include "VulkanRHI/VulkanRHI.h"
#include "Application/Launch.h"
#include "Utilities/CommandLineArgs.h"
#include <atomic>

namespace Wanli
{
    struct EngineCreateInfo
    {
        CommandLineArgs commandLineArgs;
        ApplicationCreateInfo appInfo;
        VulkanRHICreateInfo vulkanInfo;
    };

    class DLLDECL Engine
    {
    public:
        static Engine* Get();

        bool Initialize(const EngineCreateInfo& info);
        void Destroy();
        void Tick(double time, double delta);
        bool ShouldQuit() const;

        VulkanRHI* GetVulkanRHI() const;
        IApplication* GetApplication() const;

        void AddRequiredInstanceExtensions(const char* name);
        void AddRequiredDeviceExtension(const char* name);
        const PCCHArray& GetRequiredInstanceExtensions() const;
        const PCCHArray& GetRequiredDeviceExtensions() const;

    private:
        EngineCreateInfo mCreateInfo;

        SharedPtr<IApplication> mApplication;
        SharedPtr<VulkanRHI> mVulkanRHI;

        PCCHArray mRequiredInstanceExtensions;
        PCCHArray mRequiredDeviceExtensions;

        std::atomic_bool mQuitFlag = false;
    };
}