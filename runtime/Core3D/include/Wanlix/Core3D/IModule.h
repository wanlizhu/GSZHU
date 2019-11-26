#pragma once

#include "Config.h"
#include "Wanlix/Core3D/Utility/NonCopyable.h"
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
        virtual bool Tick(float delta) { return true; }

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