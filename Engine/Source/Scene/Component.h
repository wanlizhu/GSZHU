#pragma once

#include "Core/BasicTypes.h"
#include "Utils/Delegate.h"
#include "Utils/NonCopyable.h"
#include "Utils/UniqueID.h"

namespace Wanli
{
    class IComponent : virtual public Observer
        , public NonCopyable
        , public std::enable_shared_from_this<IComponent>
    {
        friend class Entity;
    public:
        explicit IComponent(const String& name)
            : mName(name)
            , mId(UniqueID<IComponent>::Get())
        {}

        virtual ~IComponent() = default;
        virtual void Initialize() = 0;
        virtual void Update() = 0;

        inline const String& GetName() const { return mName; }
        inline bool IsEnabled() const { return mEnabled; }
        inline bool IsRemoved() const { return mRemoved; }

        inline void SetName(const String& name) { mName = name; }
        inline void SetEnabled(bool value) { mEnabled = value; }
        inline void SetRemoved(bool value) { mRemoved = value; }

        bool IsAttachedTo(Entity* entity) const;
        void RemoveFromEntity(Entity* entity);
        bool operator==(const IComponent& rhs) const;
        bool operator< (const IComponent& rhs) const;

    protected:
        String mName;
        Uint mId;
        bool mEnabled = true;
        bool mRemoved = false;
        Array<std::weak_ptr<Entity>> mEntities;
    };
}