#pragma once

#include "EngineConfig.h"
#include "Wanlix/Utility/NonCopyable.h"
#include <memory>
#include <typeindex>

namespace Wanlix
{
    class Context;

    class IModule : public NonCopyable
    {
    public:
        virtual ~IModule() = default;
        virtual bool Initialize() { return true; }
        virtual void Tick(float delta) {}

        template<typename T>
        bool IsInstanceOf() const { 
            return typeid(T).hash_code() == mInstanceType.hash_code();
        }

    protected:
        IModule(
            std::type_index type,
            std::weak_ptr<Context> context
        )
            : mInstanceType(type)
            , mContext(context)
        {}

    protected:
        std::type_index mInstanceType;
        std::weak_ptr<Context> mContext;
    };
}