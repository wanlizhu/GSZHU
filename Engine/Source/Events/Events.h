#pragma once

#include "Core/Common.h"
#include "Core/IModule.h"
#include "Events/IEvent.h"

namespace Wanli
{
    class WANLI_API Events 
        : public IModule::Registrar<Events, void, EModuleStage::PreUpdate>
    {
    public:
        using Config = void;
        using Callback = std::function<void(IEvent::PTR)>;

        Events();
        virtual ~Events();

        virtual void Update() override;

        template<typename T>
        inline void AddCallback(const Callback& func) 
        {
            mCallbackMap[typeid(T)].emplace_back(func);
        }

        inline void PostEvent(const IEvent::PTR& event);
        inline void SendEvent(const IEvent::PTR& event);

    protected:
        std::queue<IEvent::PTR> mEventQueue;
        HashMap<std::type_index, Array<Callback>> mCallbackMap;
    };
}