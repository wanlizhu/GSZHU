#pragma once

#include "Types.h"
#include "Wanlix/Core3D/Utility/NonCopyable.h"

namespace Wanlix
{
    class Context;

    class IModule : public NonCopyable
    {
    public:
        using Ptr      = std::shared_ptr<IModule>;
        using ConstPtr = std::shared_ptr<const IModule>;
        using WeakPtr  = std::weak_ptr<IModule>;

        virtual ~IModule() = default;
        virtual bool Initialize()      { return true; }
        virtual bool Tick(float delta) { return true; }

        template<typename T>
        bool IsInstanceOf() const { 
            return typeid(T).hash_code() == mInstanceType.hash_code();
        }

    protected:
        IModule(std::type_index type, std::weak_ptr<Context> context)
            : mInstanceType(type), mContext(context)
        {}

    protected:
        std::type_index mInstanceType;
        std::weak_ptr<Context> mContext;
    };
}