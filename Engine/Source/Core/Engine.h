#pragma once

#include "Configuration/Configuration.h"
#include "Common.h"
#include "IRenderer.h"
#include "Utils/NonCopyable.h"
#include "Utils/FrameRateController.h"
#include "Utils/Future.h"
#include "Utils/Log.h"
#include "IModule.h"
#include "IApplication.h"

namespace Wanli
{
    class WANLI_API Engine : public NonCopyable
    {
    public:
        static Engine* Get();
        
        int Run();
        Future<int> AsyncRun();
        void SetShouldQuit(bool value);
        void SetApp(IApplication::UPTR&& app);
        void Quit();

        bool IsRunning() const;
        IApplication* GetApp() const;
        ArgList const& GetArgList() const;
        Version GetVersion() const;
        FrameRateController const& GetFrameRate() const;
        FrameRateController& GetFrameRate();
        
    protected:
        void Initialize();
        void Destroy();
        void UpdateStage(EModuleStage stage);

    private:
        Version mVersion;
        ArgList mArgList;
        bool mRunning = false;
        bool mPrioritizeUpdateOverRender = false;

        IApplication::UPTR mApp;
        FrameRateController mFrameRate;
    };
}