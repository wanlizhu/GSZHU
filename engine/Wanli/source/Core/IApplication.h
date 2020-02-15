#pragma once

#include "Utilities/Delegate.h"

namespace Wanli
{
    class DLLDECL IApplication : virtual Observer
    {
    public:
        explicit IApplication(const String& name)
            : mName(name)
        {}
        virtual ~IApplication() = default;

        virtual void Start() = 0;
        virtual void Update() = 0;

        inline const char* GetName() const { return mName.c_str(); }

    protected:
        bool mStarted = false;
        String mName;
    };
}