#pragma once
#include <ZHUENGINE/Common/Config.h>
#include <ZHUENGINE/Core/ModuleManager.h>
#include <ZHUENGINE/Core/IRenderer.h>
#include <ZHUENGINE/Common/TimeDelta.h>
#include "Common/FPSCounter.h"
#include <memory>

namespace ZHU
{
    class ZHU_API Engine final : public NonCopyable
    {
    public:
        friend std::shared_ptr<Engine> std::make_shared<Engine>();
        struct Version { int major, minor, patch; };

        static bool      Initialize();
        static Engine*   Get();
        static void      Shutdown();

        int              Run();
        const Version&   GetVersion() const;
        IRenderer*       GetRenderer() const;
        void             SetRenderer(std::unique_ptr<IRenderer>&& renderer);
        int              GetFPS() const;

        template<typename T>
        bool HasModule() const { 
            return mModuleManager.HasModule<T>();
        }

        template<typename T>
        T* GetModule() const {
            return mModuleManager.GetModule<T>();
        }

        template<typename T, typename... ARGS>
        void AddModule(const IModule::EStage& stage, ARGS&&... args) {
            mModuleManager.AddModule<T>(stage, std::make_unique<T>(std::forward<ARGS>(args)...));
        }

        template<typename T>
        void RemoveModule() {
            mModuleManager.RemoveModule<T>();
        }

    private:
        Engine();

    private:
        static Version smVersion;
        static std::shared_ptr<Engine> smInstance;

        ModuleManager mModuleManager;
        std::unique_ptr<IRenderer> mRenderer;
        FPSCounter mFPS;
    };
}