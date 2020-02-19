#pragma once

#include "Core/BasicTypes.h"
#include "Core/IModule.h"
#include "Configurations/Configuration.h"
#include "Application/IApplication.h"
#include "Utilities/CommandLineArgs.h"
#include "Utilities/NonCopyable.h"
#include "Utilities/FPSCounter.h"
#include "Utilities/Future.h"
#include "Utilities/Log.h"
#include "Utilities/Traits.h"

namespace Wanli
{
    class DLLDECL Engine : public NonCopyable
    {
    public:
        static Engine* Get();
        
        int Run();
        Future<int> RunAsync();

        inline void SetApp(UniquePtr<IApplication>&& app) { mApp = std::move(app); }
        inline IApplication* GetApp() const { return mApp.get(); }
        inline FPSCounter& GetFPSCounter() { return mFPSCounter; }

        template<typename T>
        inline std::enable_if_t<std::is_base_of_v<IConfig, T>, T&>
            GetConfig()
        {
            return mConfig.GetConfig<T>();
        }

        template<typename T>
        inline std::enable_if_t<std::is_base_of_v<IModule, T>, T*>
            GetModule() const
        {
            auto it = mModules.find(typeid(T));
            auto module = (it == mModules.end() ? nullptr : it->second.get());
            return dynamic_cast<T*>(module);
        }

    protected:
        template<typename T, typename... Args>
        inline bool Register(Args... args)
        {
            using ConfigT = typename T::ConfigType;

            if constexpr (std::is_base_of_v<IModule, T>)
            {
                const auto& id = typeid(T);
                auto module = std::make_unique<T>(std::forward<Args>(args)...);
                module->Initialize(&GetConfig<ConfigT>());
                mModules[id] = std::move(module);
                return mModules[id] != nullptr;
            }
            else
            {
                LOG_ERROR("!!! Unknown register type !!!\n");
                return false;
            }
        }

    private:
        void Initialize();
        void Shutdown();
        void UpdateStage(EModuleStage stage, double elapsedTime);

    private:
        bool mRunning = false;
        Version mVersion;
        CommandLineArgs mCommandLineArgs;
        FPSCounter mFPSCounter;
        Configuration mConfig;

        UniquePtr<IApplication> mApp;
        HashMap<std::type_index, UniquePtr<IModule>> mModules;
    };
}