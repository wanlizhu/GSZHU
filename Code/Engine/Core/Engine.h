#pragma once
#include "Common/Config.h"
#include "ModuleManager.h"
#include <memory>
#include "IRenderer.h"
#include "Common/TimeDelta.h"
#include "Common/FPSCounter.h"

namespace ZHU
{
    class ZHU_API Engine : public NonCopyable
                         , public HeapCreator
    {
    public:
        typedef std::shared_ptr<Engine> SharedPtr;
        typedef std::weak_ptr<Engine> WeakPtr;
        typedef std::unique_ptr<Engine> UniquePtr;
        struct Version { int major, minor, patch; };

        static bool      Initialize();
        static Engine*   Get();
        static void      Shutdown();

        int            Run();
        const Version& GetVersion() const;
        IRenderer*     GetRenderer() const;
        void           SetRenderer(std::unique_ptr<IRenderer>&& renderer);
        int            GetFPS() const;
        void           Shutdown();

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
        static Version smVersion;
        static std::shared_ptr<Engine> smInstance;

        ModuleManager& mModuleManager;
        std::unique_ptr<IRenderer> mRenderer;
        FPSCounter mFPS;
    };
}