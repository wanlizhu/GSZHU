#pragma once

#include "Core/BasicTypes.h"
#include "Core/IModule.h"
#include "Core/Plugin.h"
#include "Core/IApplication.h"
#include "Utilities/CommandLineArgs.h"
#include "Utilities/NonCopyable.h"
#include "Utilities/ElapsedTime.h"
#include "Utilities/ChangePerSecond.h"
#include "Utilities/Future.h"
#include "VulkanRHI/VulkanRHI.h"
#include "HAL/Window.h"

namespace Wanli
{
    struct EngineCreateInfo
    {
        CommandLineArgs commandLineArgs;
        UniquePtr<IApplication> app;
        WindowCreateInfo windowCreateInfo;
        VulkanRHICreateInfo vulkanRHICreateInfo;
    };

    struct EngineTimer
    {
        DeltaTime deltaUpdate;
        DeltaTime deltaRender;
        ElapsedTime elapsedUpdate;
        ElapsedTime elapsedRender;
        ChangePerSecond FPSValue;
        ChangePerSecond UPSValue;
        Optional<float> FPSLimit;
    };

    class DLLDECL Engine : public NonCopyable
    {
    public:
        static Engine* Get();
        
        ~Engine();
        bool Initialize(const EngineCreateInfo& info);
        int Run();
        Future<int> RunAsync();

        inline const Version& GetVersion() const { return mVersion; }
        inline const CommandLineArgs& GetCommandLineArgs() const { return mCommandLineArgs; }

        inline Window* GetWindow() const { return mWindow.get(); }
        inline IApplication* GetApp() const { return mApp.get(); }
        inline VulkanRHI* GetVulkanRHI() const { return mVulkanRHI.get(); }
        inline const EngineTimer& GetEngineTimer() const { return mEngineTimer; }

        template<typename T>
        inline std::enable_if_t<std::is_base_of_v<IModule, T>, T*>
            GetModule() const 
        {
            auto it = mModules.find(typeid(T));
            auto module = (it == mModules.end() ? nullptr : it->second.get());
            assert(module != nullptr);
            return dynamic_cast<T*>(module);
        }

    private:
        Engine();
        void Destroy();
        void LoadConfig();
        void LoadPlugins();
        void UpdateStage(EModuleStage stage);

    private:
        bool mRunning = false;
        Version mVersion;
        CommandLineArgs mCommandLineArgs;

        UniquePtr<IApplication> mApp;
        UniquePtr<Window> mWindow;
        UniquePtr<VulkanRHI> mVulkanRHI;
        HashMap<std::type_index, UniquePtr<IModule>> mModules;

        EngineTimer mEngineTimer;
    };
}