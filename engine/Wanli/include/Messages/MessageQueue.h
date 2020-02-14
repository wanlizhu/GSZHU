#pragma once

#include "BasicTypes.h"
#include "IModule.h"

#ifdef SendMessage
#undef SendMessage
#endif

namespace Wanli
{
    enum class EMessageType
    {
        None = 0,
    };

    class DLLDECL MessageQueue 
        : public IModule::Registrar<MessageQueue>
    {
        struct Message
        {
            EMessageType type = EMessageType::None;
            Optional<std::any> data1 = std::nullopt;
            Optional<std::any> data2 = std::nullopt;
        };
    public:
        using Callback = std::function<bool(Optional<std::any>&, Optional<std::any>&)>;

        virtual void Update() override final;
        void SendMessage(EMessageType type,
                         Optional<std::any> data1 = std::nullopt, 
                         Optional<std::any> data2 = std::nullopt);
        void AddCallback(EMessageType type, Callback callback);
        void RemoveCallbacks(EMessageType type);
        void RemoveCallbacks();
        
    private:
        std::queue<UniquePtr<Message>> mMessages;
        HashMap<EMessageType, Array<Callback>> mCallbacks;
    };
}