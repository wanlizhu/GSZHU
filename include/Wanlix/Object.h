#pragma once

#include "Wanlix/Common.h"

namespace Wanlix
{
    class Object : public std::enable_shared_from_this<Object>
                 , public NonCopyable
    {
    public:
        Object(String const& name, Object* parent = nullptr);
        Object(Object&& obj);
        Object& operator=(Object&& obj);
        virtual ~Object();

        void SetObjectName(String const& name);
        String const& GetObjectName() const;
        void SetParent(Object* parent);
        Object* GetParent() const;
        void SetProperty(String const& name, Variant const& value);
        Variant GetProperty(String const& name) const;

        Array<Object*> const& Children() const;
        Object* FindChild(String const& name) const;
        Array<Object*> FindChildren(String const& rgexName) const;

        Uint StartTimer(Int intervalMs);
        void KillTimer(Uint timerId);

    protected:
        virtual void TimerEvent(Uint timerId);

    private:
        String mObjectName;
        Object* mParent = nullptr;
        StringMap<Variant> mProperties;
    };
}