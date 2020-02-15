#pragma once

#include "BasicTypes.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    enum class EModuleStage : Uint8
    {
        Never,
        Always,
        PreUpdate,
        Update,
        PostUpdate,
        Render,
    };

    class DLLDECL IModule : public NonCopyable
        , public std::enable_shared_from_this<IModule>
    {
    public:
        explicit IModule(EModuleStage stage, const char* name)
            : mModuleStage(stage)
            , mName(name)
        {}

        virtual ~IModule() = default;
        virtual void Update() = 0;

        inline EModuleStage GetModuleStage() const { return mModuleStage; }

    private:
        const char* mName = nullptr;
        EModuleStage mModuleStage;
    };
}