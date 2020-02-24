#pragma once

#include "Scenes/IComponent.h"

namespace Wanli
{
    class Entity : public NonCopyable
        , public std::enable_shared_from_this<Entity>
    {
    public:
        explicit Entity(const String& name)
            : mId(UniqueID<Entity>::Get())
            , mName(name)
        {}

        virtual ~Entity() = default;
        virtual bool Initialize();
        virtual void Update();
        virtual bool LoadPrefab(const Path& path);

        inline const String& GetName() const { return mName; }
        inline void SetName(const String& name) { mName = name; }
        inline Uint GetID() const { return mId; }
        inline bool IsRemoved() const { return mRemoved; }
        inline void SetRemoved(bool value) { mRemoved = value; }

        Entity& AddComponent(IComponent* comp);
        Entity& RemoveComponent(IComponent* comp);
        Entity& RemoveComponent(const String& name);
        IComponent* GetComponent(const String& name) const;

        template<typename T, typename... _Args_>
        inline Entity& AddComponent(_Args_... args)
        {
            auto comp = std::make_shared<T>(std::forward<_Args_>(args)...);
            mComponents.push_back(comp);
            return *this;
        }

        template<typename T>
        inline Entity& RemoveComponent()
        {
            for (auto& it = mComponents.begin(); it != mComponents.end();)
            {
                if (dynamic_cast<T*>((*it).get()) != nullptr)
                {
                    (*it)->RemoveFromEntity(this);
                    it = mComponents.erase(it);
                }
                else
                {
                    it++;
                }
            }
        }

        template<typename T>
        inline IComponent* GetComponentOf() const
        {
            for (auto& it = mComponents.begin(); it != mComponents.end();)
            {
                if (dynamic_cast<T*>((*it).get()) != nullptr)
                {
                    return (*it).get();
                }
                else
                {
                    it++;
                }
            }
        }

        template<typename T>
        inline Array<IComponent*> GetComponentsOf() const
        {
            Array<IComponent*> comps;
            for (auto& comp : mComponents)
            {
                if (dynamic_cast<T*>(comp.get()) != nullptr)
                {
                    comps.push_back(comp.get());
                }
            }
            return comps;
        }

    protected:
        Uint mId;
        String mName;
        bool mRemoved = false;
        Array<SharedPtr<IComponent>> mComponents;
    };
}