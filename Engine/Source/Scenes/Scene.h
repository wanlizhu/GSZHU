#pragma once

#include "Core/Common.h"
#include "Core/IModule.h"
#include "Utils/Delegate.h"
#include "Utils/NonCopyable.h"
#include "Utils/Frustum.h"
#include "Components/Camera.h"
#include "World.h"

namespace Wanli
{
    class WANLI_API Scene : virtual public Observer
    {
    public:
        using PTR = std::shared_ptr<Scene>;

        explicit Scene(const String& name = "Unnamed")
            : mName(name)
        {}
        virtual ~Scene() = default;

        void Close();
        bool Load(const Path& path);
        void Save(const Path& path);
        bool IsActive() const;
        bool IsPaused() const;

        inline World* GetWorld() const { return mWorld.get(); }
       
    protected:
        String mName;
        UniquePtr<World> mWorld;
    };
}