#pragma once

#include "Events/Event.h"
#include "Core/IModule.h"

namespace Wanli
{
    class DLLDECL EventModule : public IModule
    {
    public:
        using ConfigType = EventConfig;
        using Callback = std::function<void(const std::any&)>;

        EventModule()
            : IModule(EModuleStage::PreUpdate, "EventModule")
        {}

        virtual ~EventModule();
        virtual void Initialize(IConfig* config) override final;
        virtual void Update(double elapsedTime) override final;
        virtual void Shutdown() override final;

        template<typename... Args>
        void AddEventCallback(EEventType type, const std::function<void(Args...)>& func)
        {
            mEventCallbacks[type].emplace_back(func);
        }

        void RemoveEventCallbacks(EEventType type);
        void PostEvent(SharedPtr<const IEvent> event) const;

    protected:
        std::queue<SharedPtr<IEvent>> mEventQueue;
        HashMap<EEventType, Array<Callback>> mEventCallbacks;
    };
}