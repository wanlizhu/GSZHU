#pragma once

#include "Core/IModule.h"

namespace Wanli
{
    class WANLI_API Files
        : public IModule::Registrar<Files, void, EModuleStage::Render>
    {
    public:
        Files();
        virtual ~Files();

        virtual void Update() override;

        void AddSearchPath(const Path& path);
        void ClearSearchPath();

    private:
        Array<Path> mSearchPaths;
    };
}