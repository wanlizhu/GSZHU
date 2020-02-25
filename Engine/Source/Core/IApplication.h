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

        explicit IApplication(
            const String& name,
            const Version& version = { 1, 0, 0 }
        ) noexcept
            : mName(name)
            , mVersion(version)
        {}
        virtual ~IApplication() = default;

        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;

        inline String const& GetName() const { return mName; }
        inline Version const& GetVersion() const { return mVersion; }
        inline bool IsInited() const { return mInited; }

    protected:
        String mName;
        Version mVersion;
        std::atomic_bool mInited = false;
    };
}