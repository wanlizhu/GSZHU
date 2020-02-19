#pragma once

#include "BasicTypes.h"
#include "Utilities/NonCopyable.h"
#include "Utilities/UniqueID.h"
#include "Utilities/Log.h"
#include "Configurations/Configuration.h"

namespace Wanli
{
    enum class EModuleStage : Uint8
    {
        PreUpdate,
        Update,
        Render,
        PostRender,
    };

    class DLLDECL IModule : public NonCopyable
        , public std::enable_shared_from_this<IModule>
    {
    public:
        explicit IModule(EModuleStage stage, const String& name)
            : mModuleStage(stage)
            , mName(name)
            , mId(UniqueID<IModule>::Get())
        {
            LOG_DEBUG("[IModule] Construct %s\n", mName.c_str());
        }

        virtual ~IModule()
        {
            LOG_DEBUG("[IModule] Destruct %s\n", mName.c_str());
        }
        virtual void Initialize(IConfig* config) = 0;
        virtual void Update(double elapsedTime) = 0;
        virtual void Shutdown() = 0;

        inline EModuleStage GetModuleStage() const { return mModuleStage; }
        inline const String& GetModuleName() const { return mName; }
        inline Uint GetID() const { return mId; }

    private:
        Uint mId;
        String mName;
        EModuleStage mModuleStage;
    };
}