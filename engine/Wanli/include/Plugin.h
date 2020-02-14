#pragma once

#include "IModule.h"
#include "Utilities/FileObserver.h"
#include "cr/cr.h"

namespace Wanli
{
    class DLLDECL Plugin : public IModule::Registrar<Plugin>
    {
    public:
        explicit Plugin(const String& name);
        virtual ~Plugin();

        virtual void Update() override final;

    private:
        Path mLoadedPath;
        FileObserver mFileObserver;
        UniquePtr<cr_plugin> mCrPlugin;
        bool mShouldUpdate = false;
    };
}