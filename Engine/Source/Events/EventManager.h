#pragma once

#include "Core/Common.h"
#include "Core/IModule.h"
#include "Events/IEvent.h"

namespace Wanli
{
    class WANLI_API EventManager 
        : public IModule::Registrar<EventManager, EModuleStage::PreUpdate>
    {
    public:
        using Callback = std::function<void(IEvent::PTR)>;

        EventManager() = default;
        virtual ~EventManager() = default;

        virtual void Create() override;
        virtual void Update() override;
        virtual void Destroy() override;

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