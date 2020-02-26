#pragma once

#include "Core/Common.h"
#include "Utils/Delegate.h"

namespace Wanli
{
    class WANLI_API IApplication : virtual public Observer
    {
    public:
        friend class Engine;
        using UPTR = std::unique_ptr<IApplication>;

        IApplication() = default;
        virtual ~IApplication() = default;

        virtual void Update() = 0;

        inline Version const& GetVersion() const { return mVersion; }

    protected:
        Version mVersion = { 1, 0, 0 };
    };
}