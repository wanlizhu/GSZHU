#pragma once

#include "IModule.h"
#include "Utilities/FileObserver.h"
#include "cr/cr.h"

namespace Wanli
{
    class DLLDECL PluginModule : public IModule
    {
    public:
        explicit PluginModule(const String& name);
        virtual ~PluginModule();

        virtual bool Initialize() override final;
        virtual void Update(double elapsedTime) override final;

    private:
        Path mLoadedPath;
        FileObserver mFileObserver;
        UniquePtr<cr_plugin> mCrPlugin;
        bool mShouldUpdate = false;
    };
}