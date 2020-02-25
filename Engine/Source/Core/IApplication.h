#pragma once

#include "Core/Common.h"
#include "Utils/Delegate.h"

namespace Wanli
{
    class WANLI_API IApplication : virtual public Observer
    {
    public:
        friend class Engine;

        explicit IApplication(
            const String& name,
            const Version& version = { 1, 0, 0 }
        ) noexcept
            : mName(name)
            , mVersion(version)
        {}
        virtual ~IApplication() = default;

        virtual void Create() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;

        inline const String& GetName() const { return mName; }
        inline const Version& GetVersion() const { return mVersion; }
        inline bool IsInited() const { return mInited; }

    protected:
        String mName;
        Version mVersion;
        std::atomic_bool mInited = false;
    };
}