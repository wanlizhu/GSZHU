#include "Engine.h"

namespace Wanli
{
    Engine* Engine::Get()
    {
        static Engine instance;
        return &instance;
    }

    bool Engine::Initialize(const EngineCreateInfo& info)
    {
        mCreateInfo = info;

        mApplication = MakeApplication();
        if (mApplication->Initialize(info.appInfo))
        {
            mVulkanRHI = std::make_shared<VulkanRHI>();
            if (mVulkanRHI->Initialize(info.vulkanInfo))
            {
                return true;
            }
        }

        return false;
    }

    void Engine::Destroy();
    void Engine::Tick(double time, double delta);
    bool Engine::ShouldQuit() const;

    VulkanRHI* Engine::GetVulkanRHI() const;
    IApplication* Engine::GetApplication() const;

    void Engine::AddRequiredInstanceExtensions(const char* name);
    void Engine::AddRequiredDeviceExtension(const char* name);
    const PCCHArray& Engine::GetRequiredInstanceExtensions() const;
    const PCCHArray& Engine::GetRequiredDeviceExtensions() const;
}