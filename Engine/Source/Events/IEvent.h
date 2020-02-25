#pragma once

#include "Core/Common.h"
#include "Events/EventPool.h"

namespace Wanli
{
    class IEvent : public std::enable_shared_from_this<IEvent>
    {
    public:
        using PTR = std::shared_ptr<IEvent>;

        virtual ~IEvent();
        std::type_index GetTypeIndex() const;;
        int GetTypeSize() const;
        void PostEvent();
        void SendEvent();

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