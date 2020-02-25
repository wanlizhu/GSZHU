#pragma once

#include "Core/Common.h"
#include "Events/EventPool.h"

namespace Wanli
{
    class WANLI_API IEvent : public std::enable_shared_from_this<IEvent>
    {
    public:
        using PTR = std::shared_ptr<IEvent>;

        virtual ~IEvent();
        int GetTypeSize() const;
        std::type_index GetTypeIndex() const;
        void PostEvent();
        void SendEvent();

        template<typename T>
        inline bool IsInstanceOf()
        {
            return std::type_index(typeid(T)) == mTypeIndex;
        }

    protected:
        IEvent(const std::type_index& type, int size) noexcept
            : mTypeIndex(type)
            , mTypeSize(size)
        {}

    protected:
        const std::type_index mTypeIndex;
        const int mTypeSize;
    };
}