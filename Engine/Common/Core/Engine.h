#pragma once
#include <Common/Config.h>
#include <Common/Core/ModuleManager.h>
#include <Common/Core/IRenderer.h>
#include <Common/Utilities/NonCopyable.h>
#include <Common/Utilities/TimeDelta.h>
#include <memory>

namespace ZHU
{
    class ZHU_API Engine final : public NonCopyable
    {
    public:
        struct Version { int major, minor, patch; };

        static bool      Initialize();
        static Engine*   Get();
        static void      Shutdown();

        int              Run();
        IRenderer*       GetRenderer() const;
        void             SetRenderer(std::unique_ptr<IRenderer>&& renderer);

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
        static std::shared_ptr<Engine> smInstance;

        ModuleManager mModuleManager;
        std::unique_ptr<IRenderer> mRenderer;
    };
}